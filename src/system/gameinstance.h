#ifndef GAMEINSTANCE_H
#define GAMEINSTANCE_H

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/variant.hpp"

using namespace godot;

// Game State Control
enum class GameState : uint8_t
{
	Menu = 0,
	InGame = 1,
	Editor = 2,
	Initializing = 3
};

class GameInstance : public Node
{
	GDCLASS(GameInstance, Node)

private:

	int score = 0;

	GameState currentgamestate = GameState::Initializing;

protected:

	static void _bind_methods();

public:

	GameInstance();
	~GameInstance();

	bool isRunningInEditor();

	void _notification(int p_what);

	virtual void _ready() override final;

	void print_type(const Variant &p_variant) const;

	void add_score_for_player(int p_amount);
	int get_score_for_player() const;
	void reset_game();
};

#endif // GAMEINSTANCE_H
