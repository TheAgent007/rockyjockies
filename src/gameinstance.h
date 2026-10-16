#pragma once

#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/variant.hpp"

using namespace godot;

class gameinstance : public Node
{
	GDCLASS(gameinstance, Node)

private:

	int score = 0;

protected:
	static void _bind_methods();

public:

	gameinstance();
	~gameinstance();

	void print_type(const Variant &p_variant) const;

	void add_score(int p_amount);
	int get_score() const;
	void reset_game();
};
