#include "player_character.h"

#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/classes/resource_loader.hpp"
#include "godot_cpp/classes/packed_scene.hpp"

void player_character::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_speed", "in_speed"), &player_character::set_speed);
	ClassDB::bind_method(D_METHOD("get_speed"), &player_character::get_speed);

	ClassDB::bind_method(D_METHOD("set_friction", "in_friction"), &player_character::set_friction);
	ClassDB::bind_method(D_METHOD("get_friction"), &player_character::get_friction);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "friction"), "set_friction", "get_friction");
}

player_character::player_character()
{
	set_motion_mode(godot::CharacterBody3D::MOTION_MODE_FLOATING);
}

player_character::~player_character()
{

}

void player_character::_notification(int p_what)
{

}


void player_character::_ready()
{

	if (!get_tree()->get_edited_scene_root()->has_node("CollisionShape"))
	{
		collision_shape = memnew(CollisionShape3D);

		Ref<BoxShape3D> box_shape = memnew(BoxShape3D);
		box_shape->set_size(Vector3(1.5, 1.5, 3.5));

		collision_shape->set_shape(box_shape);
		collision_shape->set_position(Vector3(0.0, 0.0, 0.0));
		collision_shape->set_name("CollisionShape");

		add_child(collision_shape);
	}

	if (!get_tree()->get_edited_scene_root()->has_node("CollisionShape/RocketVehicleMesh"))
	{
		rocket_vehicle_mesh = memnew(Node3D);
		rocket_vehicle_mesh->set_position(Vector3(0.0, -1.f, 0.0));
		rocket_vehicle_mesh->set_name("RocketVehicleMesh");

		collision_shape->add_child(rocket_vehicle_mesh);

		if (is_inside_tree())
		{
			Node *scene_root = get_tree()->get_edited_scene_root();

			if (scene_root)
			{
				collision_shape->set_owner(scene_root);
				rocket_vehicle_mesh->set_owner(scene_root);
			}
		}
	}

	if (Node3D* rocketmesh_noderef = get_tree()->get_edited_scene_root()->get_node<Node3D>("CollisionShape/RocketVehicleMesh"))
	{
		// load mesh, can we async this?
		load_rocket_mesh(rocketmesh_noderef);
	}
}

void player_character::load_rocket_mesh(Node3D *mesh_node)
{
	// Load the resource from the res:// path
	Ref<PackedScene> rocket_mesh = ResourceLoader::get_singleton()->load("res://Player/Meshes/boat-speed-a.glb");

	if (rocket_mesh.is_valid())
	{
		Node3D *glb_instance = Object::cast_to<Node3D>(rocket_mesh->instantiate());

		if (glb_instance != nullptr)
		{
			mesh_node-> add_child(glb_instance);
			glb_instance->set_position(Vector3(0.0, 0.0, 0.0));
		}
	}
}

void player_character::_physics_process(double delta)
{

}
