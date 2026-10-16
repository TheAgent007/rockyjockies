#include "gameinstance.h"

#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/variant/utility_functions.hpp"

gameinstance::gameinstance()
{
	// Construct
}

gameinstance::~gameinstance()
{
	// Destruct
}

void gameinstance::_bind_methods()
{
	godot::ClassDB::bind_method(D_METHOD("print_type", "variant"), &gameinstance::print_type);

	ClassDB::bind_method(D_METHOD("add_score", "amount"), &gameinstance::add_score);
	ClassDB::bind_method(D_METHOD("get_score"), &gameinstance::get_score);
	ClassDB::bind_method(D_METHOD("reset_game"), &gameinstance::reset_game);
}

void gameinstance::print_type(const Variant &p_variant) const
{
	print_line(vformat("Type: %d", p_variant.get_type()));
}

void gameinstance::add_score(int p_amount)
{
	score += p_amount;
	UtilityFunctions::print("Score updated: ", score);
}

int gameinstance::get_score() const
{
	return score;
}

void gameinstance::reset_game()
{
	score = 0;
	UtilityFunctions::print("Game reset.");
}
