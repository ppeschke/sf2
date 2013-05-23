#ifndef LAUNCHER_H
#define LAUNCHER_H

#include "Weapon.h"

enum weaponType {shootMissile, shootProjectile, shootDrone};

class Launcher : public Weapon
{
public:
	Launcher(unsigned int index, Player* o, renderableType mesh, Base* follow, Vec2D loc, Vec2D dir, float Rof, weaponType wT, bool collides = false, bool draw = false);
	virtual ~Launcher(void);

	virtual void shoot();
	virtual void run(float deltaTime);
	virtual void endStep(float deltaTime);
	virtual void Kill();

	weaponType shootWhat;
};

#endif