#ifndef WORLDINFO_H
#define WORLDINFO_H

#include "system/gameinstance.h"
#include "game/mode.h"

#include "godot_cpp/classes/node.hpp"

using namespace godot;

class WorldInfo : public Node
{
	GDCLASS(WorldInfo, Node)

private:
	String mode_type = "";

protected:

	static void _bind_methods();

public:

	void _get_property_list(List<PropertyInfo>* p_list) const;

	void set_mode(const String& p_mode_class)
	{
		mode_type = p_mode_class;
	}
	String get_selected_mode() const
	{
		return mode_type;
	}
};

#endif // WORLDINFO_H

