#pragma once

#include "godot_cpp/classes/character_body3d.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/variant.hpp"

using namespace godot;

class player_character : public CharacterBody3D
{
	GDCLASS(player_character, CharacterBody3D)

protected:
	static void _bind_methods();

public:
	player_character();
	~player_character();

	virtual void _ready() override;


};
