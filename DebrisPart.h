#ifndef DEBRISPART_H
#define DEBRISPART_H

#include "Vec2D.h"
#include "Mesh.h"

class DebrisPart
{
public:
	DebrisPart(Mesh* m, float lifetime, Vec2D position, Vec2D velocity, Vec2D rotationPoint, Vec2D mp, float rotationSpeed);
	~DebrisPart(void);

	void run(float deltaTime);

	Mesh* m;
	float lifetime;
	float age;
	Vec2D pos;
	Vec2D vel;
	float rot;
	Vec2D rotationpoint;
	Vec2D midpoint;
	float rotationspeed;
	bool dead;
};

#endif