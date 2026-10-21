#include "game/gamemode.h"
#include "player/player_spawnpoint.h"
#include "player/player_character.h"

#include "godot_cpp/classes/multiplayer_api.hpp"

#include "godot_cpp/classes/resource_loader.hpp"
#include "godot_cpp/classes/packed_scene.hpp"

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/core/class_db.hpp"

#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/variant/typed_array.hpp"

void GameMode::_bind_methods()
{
	// these should be called from game instance depending on init type, but exposing to gd just incase
	ClassDB::bind_method(D_METHOD("initialize_for_singleplayer"), &GameMode::initialize_for_singleplayer);
	ClassDB::bind_method(D_METHOD("initialize_for_multiplayer"), &GameMode::initialize_for_multiplayer);
}

void GameMode::_ready()
{
	UtilityFunctions::print("Game Mode Ready.");

	gameinstance_ref = Object::cast_to<GameInstance>(Engine::get_singleton()->get_singleton("GameInstance"));

	if (gameinstance_ref)
	{
		if (!gameinstance_ref->isRunningInEditor())
		{
			switch (gameinstance_ref->get_gamestate())
			{
				case GameState::Editor_InGame:
				case GameState::InGame_SinglePlayer:
					initialize_for_singleplayer();
					break;
				case GameState::InGame_Multiplayer_Server:
					initialize_for_multiplayer();
					break;
			}
		}
	}
}

void GameMode::initialize_for_singleplayer()
{
	spawn_player(1);
}

void GameMode::initialize_for_multiplayer()
{
	Ref<MultiplayerAPI> mp = get_multiplayer();
	if (mp.is_valid() && mp->is_server())
	{
		mp->connect("peer_connected", Callable(this, "on_peer_connected"));
		mp->connect("peer_disconnected", Callable(this, "on_peer_disconnected"));

		spawn_player(1); // listen server player spawn
	}
}

void GameMode::spawn_player(int64_t peer_id)
{
	Node* world = get_tree()->get_current_scene();

	if (world)
	{
		TypedArray<PlayerSpawnPoint> player_spawns = TypedArray<PlayerSpawnPoint>();

		// find player spots and spawn player
		gameinstance_ref->find_nodes_by_type<PlayerSpawnPoint>(world, player_spawns);

		if (player_spawns.size() > 0)
		{
			UtilityFunctions::print("Found Player Starts!");

			for (int32_t i = 0; i < player_spawns.size(); i++)
			{
				PlayerSpawnPoint* spawnpoint = godot::Object::cast_to<PlayerSpawnPoint>(player_spawns[i]);

				if (spawnpoint && !spawnpoint->GetPlayerSpawnPointTaken())
				{
					ResourceLoader* loader = ResourceLoader::get_singleton();
					Ref<PackedScene> player_scene = loader->load("res://Player/JockyPlayer.tscn");

					if (player_scene.is_valid())
					{
						PlayerCharacter* new_player = cast_to<PlayerCharacter>(player_scene->instantiate());

						if (new_player)
						{
							new_player->set_name(String::num_int64(peer_id));
							new_player->call("set_multiplayer_authority", peer_id);

							spawnpoint->MarkPlayerSpawnPointTaken(new_player);
							add_child(new_player);

							new_player->set_position(spawnpoint->get_global_position());
						}
					}
				}
			}
		}
	}
}
