#ifndef GAMEMODE_H
#define GAMEMODE_H

#include "system/gameinstance.h"
#include "game/mode.h"

using namespace godot;

class GameMode : public Mode
{
	GDCLASS(GameMode, Mode)

public:

	virtual void _ready() override;

	void initialize_for_singleplayer();
	void initialize_for_multiplayer();

	void on_client_connected(int64_t peer_id);

	void on_client_disconnected(int64_t peer_id);

	void spawn_player(int64_t peer_id);

private:

	GameInstance* gameinstance_ref = nullptr;

protected:

	static void _bind_methods();
};

#endif // GAMEMODE_H