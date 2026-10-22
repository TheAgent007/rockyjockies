#ifndef GRAPPLING_HOOK_H
#define GRAPPLING_HOOK_H

#include "godot_cpp/classes/node.hpp"

using namespace godot;

class GrapplingHook : public Node
{
	GDCLASS(GrapplingHook, Node)

public:

	static void _bind_methods();
};

#endif // GRAPPLING_HOOK_H

