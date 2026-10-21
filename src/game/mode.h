#ifndef MODE_H
#define MODE_H

#include "godot_cpp/classes/node.hpp"

using namespace godot;

class Mode : public Node
{
	GDCLASS(Mode, Node)

protected:

	static void _bind_methods();

public:
};

#endif // MODE_H

