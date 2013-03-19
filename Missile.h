#ifndef MISSILE_H
#define MISSILE_H

#include "Base.h"

class Missile : public Base
{
public:
	Missile(unsigned int index, Player* o, renderableType mesh, Vec2D loc, Vec2D dir, bool collides = true, bool draw = true);
	virtual ~Missile(void);

	virtual void run(float deltaTime);

	float speed;
	float turnForce;
	int fuel;
	int mass;
	bool spent;
};

#endif