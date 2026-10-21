#ifndef GAMEINSTANCE_H
#define GAMEINSTANCE_H

#include "godot_cpp/classes/object.hpp"
#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "godot_cpp/variant/typed_array.hpp"

using namespace godot;

// Game State Control
enum class GameState : uint8_t
{
	Menu = 0,
	InGame_SinglePlayer = 1,
	InGame_Multiplayer_Server = 2,
	InGame_Multiplayer_Client = 3,
	Editor = 2,
	Initializing = 3
};

class GameInstance : public Object
{
	GDCLASS(GameInstance, Object)

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

	void check_initialization();

	void _initialize();

	void _on_node_added();
	
	template <typename T>
	void find_nodes_by_type(Node* p_root, TypedArray<T>& r_out)
	{
		if (!p_root) return;

		// Check if the current node matches type T
		T* typed_node = Object::cast_to<T>(p_root);
		if (typed_node)
		{
			r_out.push_back(typed_node);
		}

		// Recurse through all children
		int count = p_root->get_child_count();
		for (int i = 0; i < count; i++)
		{
			Node* child = p_root->get_child(i);
			find_nodes_by_type<T>(child, r_out);
		}
	}


	void add_score_for_player(int p_amount);
	int get_score_for_player() const;

	bool is_tree_initialized = false;
};

#endif // GAMEINSTANCE_H
