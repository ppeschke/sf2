#ifndef DAMAGECIRCLE_H
#define DAMAGECIRCLE_H

#include "Base.h"

class DamageCircle : public Base
{
public:
	DamageCircle(unsigned int index, Player* player, Vec2D location, Vec2D direction, float life, bool collides = true, bool draw = true);
	virtual ~DamageCircle(void);

	virtual void run(float deltaTime);
	virtual void endStep(float deltaTime);
	virtual void Kill();

	float lifetime;
	int hitpoints;
};

#endif