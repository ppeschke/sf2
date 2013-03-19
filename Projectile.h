#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Base.h"

class Projectile : public Base
{
public:
	Projectile(unsigned int index, Player* o, renderableType mesh, Vec2D loc, Vec2D dir, bool collides = true, bool draw = true);
	virtual ~Projectile(void);

	virtual void run(float deltaTime);

	unsigned int lifetime;
};

#endif