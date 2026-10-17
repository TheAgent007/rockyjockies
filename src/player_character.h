#pragma once

#include "godot_cpp/classes/character_body3d.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/variant.hpp"

#include "godot_cpp/classes/character_body3d.hpp"
#include "godot_cpp/classes/collision_shape3d.hpp"
#include "godot_cpp/classes/box_shape3d.hpp"

#include "godot_cpp/classes/label.hpp"

using namespace godot;

class player_character : public CharacterBody3D
{
	GDCLASS(player_character, CharacterBody3D)

private:

	CollisionShape3D* collision_shape = nullptr;
	Node3D* rocket_vehicle_mesh = nullptr;

	float speed = 300.f;
	float friction = 10.f;

protected:

	static void _bind_methods();

public:

	player_character();
	~player_character();

	void _notification(int p_what);

	virtual void _ready() override;

	void load_rocket_mesh(Node3D *mesh_node);

	void _physics_process(double delta) override;

	void set_speed(const float in_speed)
	{
		speed = in_speed;
	};

	float get_speed() const
	{
		return speed;
	}

	void set_friction(const float in_friction)
	{
		friction = in_friction;
	};

	float get_friction() const
	{
		return friction;
	}
};
