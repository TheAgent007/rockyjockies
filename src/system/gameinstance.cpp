#include "system/gameinstance.h"
#include "player/player_spawnpoint.h"
#include "game/gamemode.h"

#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/classes/window.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/os.hpp"
#include "godot_cpp/classes/node.hpp"

using namespace godot;

// Expose these to GD script
void GameInstance::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("add_score", "amount"), &GameInstance::add_score_for_player);
	ClassDB::bind_method(D_METHOD("get_score"), &GameInstance::get_score_for_player);

	ClassDB::bind_method(D_METHOD("check_initialization"), &GameInstance::check_initialization);
}

GameInstance::GameInstance() 
{
	if (isRunningInEditor())
	{
		currentgamestate = GameState::Editor;
	}
}

GameInstance::~GameInstance()
{
	// Destruct
}

void GameInstance::_notification(int p_what)
{
	UtilityFunctions::print("Init Game." + String::num_int64(p_what));

	Callable check_init = Callable(this, "check_initialization");
	check_init.call_deferred();
}

void GameInstance::check_initialization()
{
	MainLoop* main_loop = Engine::get_singleton()->get_main_loop();
	SceneTree* tree = Object::cast_to<SceneTree>(main_loop);

	if (tree)
	{
		_initialize();
	}
	else
	{
		Callable(this, "check_initialization").call_deferred();
	}
}

void GameInstance::_initialize()
{
	MainLoop* main_loop = Engine::get_singleton()->get_main_loop();
	SceneTree* tree = Object::cast_to<SceneTree>(main_loop);

	// spawn base game mode
	Node* current_scene = tree->get_current_scene();

	if (current_scene)
	{
		UtilityFunctions::print("Creating Game Mode.");
		GameMode* new_GameMode = memnew(GameMode);
		current_scene->add_child(new_GameMode);
	}

	is_tree_initialized = true;
}

void GameInstance::_on_node_added()
{

}

bool GameInstance::isRunningInEditor()
{
	bool is_editor_build = OS::get_singleton()->has_feature("editor");
	bool is_editor_gui = Engine::get_singleton()->is_editor_hint();

	if (is_editor_build && !is_editor_gui)
	{
		return false;
	}

	return true;
}


void GameInstance::add_score_for_player(int p_amount)
{
	score += p_amount;
	UtilityFunctions::print("Score updated: ", score);
}

int GameInstance::get_score_for_player() const
{
	return score;
}