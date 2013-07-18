#ifndef WARPBEACON_H
#define WARPBEACON_H

#include "Base.h"
class WarpBeacon : public Base
{
public:
	WarpBeacon(unsigned int index, Player* p, Vec2D location, Vec2D direction, float time, bool collides = false, bool draw = true);
	virtual ~WarpBeacon(void);

	virtual void run(float deltaTime);
	virtual void Kill();

	float timer;
};

#endif