#ifndef HEALINGCIRCLE_H
#define HEALINGCIRCLE_H

#include "Base.h"

class HealingCircle : public Base
{
public:
	HealingCircle(unsigned int index, Player* player, Vec2D location, Vec2D direction, float life, bool collides = true, bool draw = true);
	virtual ~HealingCircle(void);

	virtual void run(float deltaTime);
	virtual void Kill();

	float lifetime;
	unsigned int hitpoints;	//for giving to other ships
};

#endif