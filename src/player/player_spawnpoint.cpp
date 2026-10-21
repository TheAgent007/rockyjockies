#include "player/player_spawnpoint.h"

#include "godot_cpp/variant/transform3d.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/core/class_db.hpp"

PlayerSpawnPoint::PlayerSpawnPoint()
{
	set_process_unhandled_input(false);
}

PlayerSpawnPoint::~PlayerSpawnPoint() {}

void PlayerSpawnPoint::MarkPlayerSpawnPointTaken(PlayerCharacter* usedby)
{
}

void PlayerSpawnPoint::_bind_methods()
{

}