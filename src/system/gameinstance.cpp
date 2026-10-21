#include "system/gameinstance.h"
#include "player/player_spawnpoint.h"
#include "game/gamemode.h"
#include "game/menumode.h"

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
	ClassDB::bind_method(D_METHOD("check_initialization"), &GameInstance::check_initialization);
}

GameInstance::GameInstance() 
{
	if (isRunningInEditor(true))
	{
		currentgamestate = GameState::Editor_InGame;
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

	if (tree) // we can initialize now since the tree exists
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
	if (!is_initialized)
	{
		MainLoop* main_loop = Engine::get_singleton()->get_main_loop();
		SceneTree* tree = Object::cast_to<SceneTree>(main_loop);

		// spawn base game mode
		Node* current_scene = tree->get_current_scene();

		if (current_scene)
		{
			if (currentgamestate == GameState::Editor_InGame)
			{
				UtilityFunctions::print("Creating Game Mode.");
				GameMode* new_GameMode = memnew(GameMode);
				current_scene->add_child(new_GameMode);
			}
			else if(currentgamestate == GameState::Menu)
			{
				UtilityFunctions::print("Creating Main Menu Mode.");

				MenuMode* new_MenuMode = memnew(MenuMode);
				current_scene->add_child(new_MenuMode);
			}
		}

		is_initialized = true;
	}
}

bool GameInstance::isRunningInEditor(bool return_editorgame)
{
	bool is_editor_build = OS::get_singleton()->has_feature("editor") && !return_editorgame;
	bool is_editor_gui = Engine::get_singleton()->is_editor_hint();

	if (is_editor_build && !is_editor_gui)
	{
		return false;
	}

	return true;
}
