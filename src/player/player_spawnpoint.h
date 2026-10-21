#ifndef PLAYER_SPAWNPOINT_H
#define PLAYER_SPAWNPOINT_H

#include "player/player_character.h"
#include "godot_cpp/classes/node3d.hpp"

using namespace godot;

class PlayerSpawnPoint : public Node3D
{
	GDCLASS(PlayerSpawnPoint, Node3D)

public:

	PlayerSpawnPoint();
	~PlayerSpawnPoint();

	void MarkPlayerSpawnPointTaken(PlayerCharacter* usedby);

	const bool GetPlayerSpawnPointTaken() const
	{
		if (takenby)
		{
			return true;
		}

		return false;
	}

protected:

	static void _bind_methods();

	PlayerCharacter* takenby = nullptr;
};

#endif // PLAYER_SPAWNPOINT_H