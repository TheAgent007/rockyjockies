#ifndef GAMEMODE_H
#define GAMEMODE_H

#include "system/gameinstance.h"
#include "godot_cpp/classes/node.hpp"

using namespace godot;

class GameMode : public Node
{
	GDCLASS(GameMode, Node)

public:

	virtual void _ready() override;

	void initialize_for_singleplayer();
	void initialize_for_multiplayer();

	void spawn_player(int64_t peer_id);

private:

	GameInstance* gameinstance_ref = nullptr;

protected:

	static void _bind_methods();
};

#endif // GAMEMODE_H