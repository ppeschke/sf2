#ifndef TURRET_H
#define TURRET_H

#include "weapon.h"

class Turret : public Weapon
{
public:
	Turret(unsigned int index, Player* o, renderableType mesh, Base* follow, Vec2D loc, Vec2D dir, unsigned int Rof, bool collides = false, bool draw = false);
	virtual ~Turret(void);

	virtual void shoot();
	virtual void run(float deltaTime);
};

#endif