#ifndef INDICATOR_H
#define INDICATOR_H

#include "base.h"

class Indicator : public Base
{
public:
	Indicator(unsigned int index, Player* o, renderableType mesh, Vec2D loc, Vec2D dir, bool collides = false, bool draw = true);
	virtual ~Indicator(void);
};

#endif