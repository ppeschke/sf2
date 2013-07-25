#ifndef EMPBOMB_H
#define EMPBOMB_H

#include "Base.h"
class EMPBomb : public Base
{
public:
	EMPBomb(unsigned int index, Player* player, Vec2D location, Vec2D direction, float life, bool collides = true, bool draw = true);
	virtual ~EMPBomb(void);

	virtual void run(float deltaTime);
	virtual void Kill();

	float lifetime;
};

#endif