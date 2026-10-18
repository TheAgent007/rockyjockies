#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include "system/gameinstance.h"

#include "godot_cpp/classes/input_event_mouse_motion.hpp"
#include "godot_cpp/classes/character_body3d.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/variant.hpp"

#include "godot_cpp/classes/spring_arm3d.hpp"
#include "godot_cpp/classes/collision_shape3d.hpp"
#include "godot_cpp/classes/box_shape3d.hpp"

#include "godot_cpp/classes/label.hpp"

using namespace godot;

class PlayerCharacter : public CharacterBody3D
{
	GDCLASS(PlayerCharacter, CharacterBody3D)

private:

	CollisionShape3D* collision_shape = nullptr;
	Node3D* rocket_vehicle_mesh = nullptr;
	Camera3D* camera = nullptr;
	SpringArm3D* springarm = nullptr;

	GameInstance* gameinstance_ref = nullptr;

	float max_speed = 20.f; // exposed to gd
	float max_strafe_speed = 1.0f; // exposed to gd
	float acceleration = 2.f; // expose to gd
	float decceleration = 2.f; // exposed to gd
	float friction = 3.f; // exposed to gd

	float turn_rate = 0.25f;

	float roll_rate = 0.12;
	float roll_return_rate = 6.0f;

	float roll_min = -0.25f;
	float roll_max = 0.25f;

	float camera_height = 2.0f;

	float gravity_rocket = 2.f;

	// camera look settings
	float turn_sensitivity = 0.002f;
	float look_sensitivity = 0.002f;

	float camera_pitch = 0.0f;
	Vector2 look_input_dir = Vector2();

	godot::Vector3 last_velocity_direction = godot::Vector3();

	Node3D* current_viewtarget = nullptr;

	float current_speed = 0.0f;

protected:

	static void _bind_methods();
	void _notification(int p_what);

public:

	PlayerCharacter();
	~PlayerCharacter();

	virtual void _ready() override;
	void initialize_references();

	void load_rocket_mesh(Node3D* mesh_node);

	void _physics_process(double delta) override;

	void _unhandled_input(const Ref<InputEvent> &event) override;
	void update_camera(double delta);
	void update_movement(double delta);

	// camera
	void set_viewtarget(Node3D* newViewTarget, float interp_speed);

	// gd expose start
	void set_maxspeed(const float in_speed)
	{
		max_speed = in_speed;
	};

	float get_maxspeed() const
	{
		return max_speed;
	}

	void set_max_strafe_speed(const float in_speed)
	{
		max_strafe_speed = in_speed;
	};

	float get_max_strafe_speed() const
	{
		return max_strafe_speed;
	}


	void set_decceleration(const float in_decceleration)
	{
		decceleration = in_decceleration;
	};

	float get_decceleration() const
	{
		return decceleration;
	}

	void set_acceleration(const float in_acceleration)
	{
		acceleration = in_acceleration;
	};

	float get_acceleration() const
	{
		return acceleration;
	}

	void set_friction(const float in_friction)
	{
		friction = in_friction;
	};

	float get_friction() const
	{
		return friction;
	}
	// gd expose end
};

#endif // PLAYER_CHARACTER_H