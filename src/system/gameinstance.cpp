#include "system/gameinstance.h"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/classes/window.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/os.hpp"

// Expose these to GD script
void GameInstance::_bind_methods()
{
	godot::ClassDB::bind_method(D_METHOD("print_type", "variant"), &GameInstance::print_type);

	ClassDB::bind_method(D_METHOD("add_score", "amount"), &GameInstance::add_score_for_player);
	ClassDB::bind_method(D_METHOD("get_score"), &GameInstance::get_score_for_player);
	ClassDB::bind_method(D_METHOD("reset_game"), &GameInstance::reset_game);
}

GameInstance::GameInstance() 
{
	// Construct
}

GameInstance::~GameInstance()
{
	// Destruct
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

void GameInstance::_notification(int p_what)
{
	switch (p_what)
	{

	}
}

void GameInstance::_ready()
{
	
}


void GameInstance::print_type(const Variant& p_variant) const
{
	print_line(vformat("Type: %d", p_variant.get_type()));
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

void GameInstance::reset_game()
{
	score = 0;
	UtilityFunctions::print("Game reset.");
}
