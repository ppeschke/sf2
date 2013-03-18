#include <cmath>
using namespace std;
#include "Particle.h"
#include "Vec2D.h"
#include "Global.h"

Particle::Particle()
{
	radius = 50;
	lifespan = 10;
	age = 0;
	active = false;
}

Particle::Particle(float rad, int lifetime)
{
	radius = rad;
	lifespan = lifetime;
	age = 0;
	active = false;
}

// this function sets the particle back to its original state
void Particle::reset_particle(Vec2D* loc, Vec2D* vel, Vec2D* acc, int l)
{
    active = true;
	
    position.x = loc->x;
	position.y = loc->y;
    position.z = 0.0f;

    velocity.x = vel->x;
	velocity.y = vel->y;
    velocity.z = 0.0f;

    acceleration.x = acc->x;
	acceleration.y = acc->y;
	acceleration.z = 0.0f;
    age = 0;
	lifespan = l;

    return;
}

// this function updates the particle
void Particle::run_particle()
{
	if(active)
    {
		velocity += acceleration;
		position += velocity;
		age += 1;
	}
    return;
}