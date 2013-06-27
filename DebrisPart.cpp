#include "DebrisPart.h"

DebrisPart::DebrisPart(Mesh* mesh, float lt,Vec2D position, Vec2D velocity, Vec2D rotationPoint, Vec2D mp, float rotationSpeed)
{
	m = mesh;
	midpoint = mp;
	rot = 0.0f;
	lifetime = lt;
	pos = position;
	vel = velocity;
	rotationpoint = rotationPoint;
	rotationspeed = rotationSpeed;
	dead = false;
	age = 0.0f;
}


void DebrisPart::run(float deltaTime)
{
	pos += vel * deltaTime;
	age += deltaTime;
	rot += rotationspeed * deltaTime;
	if(age > lifetime)
		dead = true;
}

DebrisPart::~DebrisPart(void)
{
	delete [] m;
}
