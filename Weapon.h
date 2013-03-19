#ifndef WEAPON_H
#define WEAPON_H

#include "Base.h"

class Weapon : public Base
{
public:
	Weapon(unsigned int index, Player*, Base* follow, Vec2D location, Vec2D direction, unsigned int ROF, bool collides = false, bool draw = false);
	virtual ~Weapon(void);
	
	virtual void shoot() = 0;
	virtual void run(float deltaTime) = 0;

	unsigned int Timer;
	unsigned int ROF;

	Vec2D coords;
	Base* follow;
};

#endif