#ifndef MENUMODE_H
#define MENUMODE_H

#include "system/gameinstance.h"
#include "game/mode.h"

using namespace godot;

class MenuMode : public Mode
{
	GDCLASS(MenuMode, Mode)

protected:

	static void _bind_methods();

public:
};

#endif // MENUMODE_H
