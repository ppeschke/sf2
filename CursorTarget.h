#ifndef CURSORTARGET_H
#define CURSORTARGET_H

#include "Base.h"

class CursorTarget : public Base
{
public:
	CursorTarget(unsigned int index, Player* o, renderableType i, Vec2D location, Vec2D direction, bool collides = true, bool draw = true);
	virtual ~CursorTarget(void);

	virtual void run(float deltaTime);

	Vec2D relCoords;
};

#endif