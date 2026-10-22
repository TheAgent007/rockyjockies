#include "player/player_character.h"

#include "godot_cpp/classes/input.hpp"
#include "godot_cpp/classes/input_event_mouse_motion.hpp"
#include "godot_cpp/classes/project_settings.hpp"

#include "godot_cpp/classes/node3d.hpp"
#include "godot_cpp/classes/camera3d.hpp"
#include "godot_cpp/variant/transform3d.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/classes/resource_loader.hpp"
#include "godot_cpp/classes/packed_scene.hpp"
#include "godot_cpp/classes/os.hpp"

using namespace godot;

void PlayerCharacter::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_maxspeed", "in_speed"), &PlayerCharacter::set_maxspeed);
	ClassDB::bind_method(D_METHOD("get_maxspeed"), &PlayerCharacter::get_maxspeed);

	ClassDB::bind_method(D_METHOD("set_max_strafe_speed", "in_speed"), &PlayerCharacter::set_max_strafe_speed);
	ClassDB::bind_method(D_METHOD("get_max_strafe_speed"), &PlayerCharacter::get_max_strafe_speed);

	ClassDB::bind_method(D_METHOD("set_decceleration", "in_decceleration"), &PlayerCharacter::set_decceleration);
	ClassDB::bind_method(D_METHOD("get_decceleration"), &PlayerCharacter::get_decceleration);

	ClassDB::bind_method(D_METHOD("set_acceleration", "in_acceleration"), &PlayerCharacter::set_acceleration);
	ClassDB::bind_method(D_METHOD("get_acceleration"), &PlayerCharacter::get_acceleration);

	ClassDB::bind_method(D_METHOD("set_friction", "in_friction"), &PlayerCharacter::set_friction);
	ClassDB::bind_method(D_METHOD("get_friction"), &PlayerCharacter::get_friction);

	ClassDB::bind_method(D_METHOD("set_turnrate", "in_turnrate"), &PlayerCharacter::set_turnrate);
	ClassDB::bind_method(D_METHOD("get_turnrate"), &PlayerCharacter::get_turnrate);

	ClassDB::bind_method(D_METHOD("set_rollrate", "in_rollrate"), &PlayerCharacter::set_rollrate);
	ClassDB::bind_method(D_METHOD("get_rollrate"), &PlayerCharacter::get_rollrate);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_speed",PROPERTY_HINT_NONE,"Max speed of Rocky Jockey"), "set_maxspeed", "get_maxspeed");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_strafe_speed"), "set_max_strafe_speed", "get_max_strafe_speed");

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "acceleration"), "set_acceleration", "get_acceleration");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "decceleration"), "set_decceleration", "get_decceleration");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "friction"), "set_friction", "get_friction");

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "turn_rate"), "set_turnrate", "get_turnrate");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "roll_rate"), "set_rollrate", "get_rollrate");
}

PlayerCharacter::PlayerCharacter()
{
	set_motion_mode(godot::CharacterBody3D::MOTION_MODE_FLOATING);

	// you can disable certain processes manuall
	// set_process_unhandled_input(false);
}

PlayerCharacter::~PlayerCharacter(){}

void PlayerCharacter::_notification(int p_what)
{
	Node::_notification(p_what);
}

void PlayerCharacter::_ready()
{
	initialize_references();
}

void PlayerCharacter::initialize_references() 
{
	UtilityFunctions::print("initializing refs");
	SpringArm3D* springarm_ref = cast_to<SpringArm3D>(get_node_or_null("CollisionShape/SpringArm3D"));
	Camera3D* camera_ref = cast_to<Camera3D>(get_node_or_null("CollisionShape/SpringArm3D/Camera"));
	RayCast3D* hover_raycast_ref = cast_to<RayCast3D>(get_node_or_null("HoverRaycast"));

	if (camera_ref)
	{
		camera = camera_ref;
		current_viewtarget = this; // set self as view target but this can be changed to anything
	}

	if (springarm_ref)
	{
		springarm = springarm_ref;
	}

	if (hover_raycast_ref)
	{
		hover_raycast = hover_raycast_ref;
	}

	gameinstance_ref = Object::cast_to<GameInstance>(Engine::get_singleton()->get_singleton("GameInstance"));
}

void PlayerCharacter::load_rocket_mesh(Node3D* mesh_node)
{
	// load the resource from the res:// path
	Ref<PackedScene> rocket_mesh = ResourceLoader::get_singleton()->load("res://Player/Meshes/boat-speed-a.glb");

	if (rocket_mesh.is_valid())
	{
		Node3D* glb_instance = Object::cast_to<Node3D>(rocket_mesh->instantiate());

		if (glb_instance != nullptr)
		{
			mesh_node-> add_child(glb_instance);
			glb_instance->set_position(Vector3(0.0, 0.0, 0.0));
		}
	}
}

void PlayerCharacter::_physics_process(double delta)
{
	Input* input = godot::Input::get_singleton();

	if (!gameinstance_ref || gameinstance_ref->isRunningInEditor())
	{
		return;
	}

	if (input)
	{
		update_camera(delta);
		update_movement(delta);
	}
}

// get mouse input
void PlayerCharacter::_unhandled_input(const Ref<InputEvent> &event) 
{
	Ref<InputEventMouseMotion> mouse_motion = event;

	if (mouse_motion.is_valid()) 
	{
		look_input_dir = mouse_motion->get_relative();
	}
}

void PlayerCharacter::update_camera(double delta)
{
	if (camera && springarm)
	{
		const Input *input = godot::Input::get_singleton();

		const Vector3 camera_offset = Vector3(0.0f, camera_height, 0.0f);
		springarm->set_global_position(get_global_position() + camera_offset);

		if (current_viewtarget)
		{
			camera->look_at(current_viewtarget->get_global_position(), Vector3(0.0f, 1.0f, 0.0f));
		}

		// get from joystick input
		const Vector2 joystick_input = input->get_vector("look_left", "look_right", "look_up", "look_down");

		if (joystick_input != Vector2())
		{
			look_input_dir = joystick_input;
		}

		if (look_input_dir != Vector2())
		{
			springarm->rotate_y(-look_input_dir.x * turn_sensitivity);

			camera_pitch -= look_input_dir.y * look_sensitivity;
			camera_pitch = UtilityFunctions::clamp(camera_pitch, UtilityFunctions::deg_to_rad(-89.0f), UtilityFunctions::deg_to_rad(89.0f));

			// get the current euler rotations, update pitch, and reapply
			Vector3 current_rotation = springarm->get_rotation();
			current_rotation.x = camera_pitch;
			springarm->set_rotation(current_rotation);
		}

		// clear camera look input
		look_input_dir = Vector2();
	}
}

void PlayerCharacter::update_movement(double delta)
{
	Vector3 velocity = get_velocity();
	double gravity = 0.0f;

	ProjectSettings* project_settings = ProjectSettings::get_singleton();

	if (project_settings)
	{
		gravity = gravity_rocket;
	}

	const Input* input = godot::Input::get_singleton();

	const Vector2 input_dir = input->get_vector("move_left", "move_right", "move_forwards", "move_backwards");

	if (camera)
	{
		Transform3D player_transform = get_global_transform();

		Vector3 forward = player_transform.basis.get_column(2); // backward
		Vector3 right = player_transform.basis.get_column(0); // right

		forward.y = 0.0f;
		right.y = 0.0f;
		forward = forward.normalized();
		right = right.normalized();

		Vector3 direction = (forward * input_dir.y);
		direction = direction.normalized();

		// cache current velocity
		Vector3 current_vel = Vector3(velocity.x, 0.f, velocity.z);

		if (input_dir.y < 0.0f) // apply forward velocity
		{
			Vector3 vel_change = (-forward * acceleration) * delta;
			current_vel += vel_change;

			current_vel.z = Math::clamp(current_vel.z, -max_speed, max_speed);
			current_vel.x = Math::clamp(current_vel.x, -max_speed, max_speed);
		}
		else // not moving just use friction?
		{
			current_vel = current_vel.move_toward(Vector3(0.0f, 0.0f, 0.0f), friction * delta);
		}

		last_velocity_direction = current_vel;
		current_speed = current_vel.length();


		// update turn rotation and roll
		const Vector3 current_velocity_local = to_local(get_global_position() + last_velocity_direction);
		const Vector3 desired_move_dir = current_vel.normalized() * -forward;

		if (desired_move_dir.length() > 0.0f) // if we are trying to turn and moving forward apply rotation
		{
			Vector3 current_rot = get_rotation();
			current_rot.y += (turn_rate * -input_dir.x) * delta;
			current_rot.z += (roll_rate * -input_dir.x) * delta;
			current_rot.z = Math::clamp(current_rot.z, roll_min, roll_max);

			set_rotation(current_rot);

			Vector3 new_forward = get_global_transform().basis.get_column(2); // get updated forward

			// update our current velocity direction since we turned
			last_velocity_direction = -new_forward * current_speed;
		}

		// roll left and right when turning
		if (Math::abs(input_dir.x) == 0.0f)
		{
			Vector3 current_rot = get_rotation();
			Vector3 target_rot = Vector3(current_rot.x, current_rot.y,0.0f);
			current_rot.z = Math::lerp_angle((double)current_rot.z, (double)target_rot.z, roll_return_rate * delta);

			set_rotation(current_rot);
		}
	}

	// finally set 2D velocity
	velocity.x = last_velocity_direction.x;
	velocity.z = last_velocity_direction.z;

	// make hover
	if (hover_raycast && hover_raycast->is_colliding())
	{
		Vector3 collision_point = hover_raycast->get_collision_point();

		const float current_height = get_global_transform().origin.distance_to(collision_point);
		const float displacement = target_hover_height - current_height;
		const float spring_force = displacement * spring_stiffness;

		const float damping_force = velocity.y * spring_damping;
		const float vertical_force = spring_force - damping_force;

		velocity.y += vertical_force * delta;
	}
	else
	{
		// apply gravity
		velocity.y -= gravity * delta;
	}

	set_velocity(velocity);
	move_and_slide();

	// check collision so we can bounce off
	int collision_count = get_slide_collision_count();

	for (int i = 0; i < collision_count; i++)
	{
		Ref<KinematicCollision3D> collision = get_slide_collision(i);
		if (collision.is_valid())
		{
			Node3D* collider = Object::cast_to<Node3D>(collision->get_collider());
			if (collider)
			{
				UtilityFunctions::print("Collided with: ", collider->get_name());

				Vector3 impact_normal = collision->get_normal();
				Vector3 position = collision->get_position(); // maybe play collision effect here

				Vector3 up = Vector3(0, 1, 0);

				Vector3 deflection_vector = last_velocity_direction.bounce(impact_normal) * impact_bounce_scale;

				// create a forward facing the hit deflection
				Basis basis = Basis::looking_at(deflection_vector.normalized(), up);

				Transform3D transform = get_transform();
				transform.basis = basis;
				set_transform(transform);

				last_velocity_direction = deflection_vector;
				set_velocity(last_velocity_direction);
				move_and_slide();
			}
		}
	}
}

void PlayerCharacter::set_viewtarget(Node3D* newViewTarget, float interp_speed)
{
	current_viewtarget = newViewTarget;
}
