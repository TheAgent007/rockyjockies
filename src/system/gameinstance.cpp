#include "system/gameinstance.h"
#include "player/player_spawnpoint.h"
#include "game/mode.h"
#include "game/gamemode.h"
#include "game/worldinfo.h"

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
			WorldInfo* scene_worldinfo = current_scene->get_node<WorldInfo>("WorldInfo");

			if (scene_worldinfo)
			{
				// get selected game mode from world info
				String mode_class = scene_worldinfo->get_selected_mode();

				Object* obj = ClassDB::instantiate(mode_class);

				if (!obj)
				{
					UtilityFunctions::push_error("Failed to instantiate mode: " + mode_class);
					spawn_default_mode();
				}
				else
				{
					Mode* new_mode = Object::cast_to<Mode>(obj);

					if (!new_mode)
					{
						memdelete(obj); // clean up if it's not a valid Node type
						UtilityFunctions::push_error("Class is not a Mode: " + mode_class);
						spawn_default_mode();
					}
					else
					{
						UtilityFunctions::print("Creating Mode: " + mode_class);
						current_scene->add_child(new_mode);
					}
				}
			}
			else
			{
				UtilityFunctions::push_error("No World Info found in scene! Creating default mode.");
				spawn_default_mode();
			}
		}

		is_initialized = true;
	}
}

void GameInstance::spawn_default_mode()
{
	MainLoop* main_loop = Engine::get_singleton()->get_main_loop();
	SceneTree* tree = Object::cast_to<SceneTree>(main_loop);

	// spawn base game mode
	Node* current_scene = tree->get_current_scene();

	currentgamestate == GameState::Editor_InGame;
	UtilityFunctions::print("Creating Default Mode.");
	GameMode* new_Mode = memnew(GameMode);
	current_scene->add_child(new_Mode);
}

const bool GameInstance::isRunningInEditor(bool return_editorgame)
{
	bool is_editor_build = OS::get_singleton()->has_feature("editor") && !return_editorgame;
	bool is_editor_gui = Engine::get_singleton()->is_editor_hint();

	if (is_editor_build && !is_editor_gui)
	{
		return false;
	}

	return true;
}

const bool GameInstance::get_is_dedicated_server()
{
	OS* os = OS::get_singleton();

	if (os && os->has_feature("dedicated_server"))
	{
		return true;
	}

	return false;
}

