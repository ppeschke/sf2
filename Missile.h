#ifndef MISSILE_H
#define MISSILE_H

#include "Base.h"

class Missile : public Base
{
public:
	Missile(unsigned int index, Player* o, renderableType mesh, Vec2D loc, Vec2D dir, bool collides = true, bool draw = true);
	virtual ~Missile(void);

	virtual void run(float deltaTime);

	float thrustForce;
	float maxSpeed;
	float turnForce;
	float fuel;
	float lifeTime;	//this lifetime only starts after the fuel runs out and is a measure to keep deep space from being crowded
					//with unexploded missiles
	int mass;
	bool spent;
};

#endif