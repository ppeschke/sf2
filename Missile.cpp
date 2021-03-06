#include "Global.h"
#include "Ship.h"
#include "Missile.h"
#include "Player.h"
#include "Team.h"

Missile::Missile(unsigned int index, Player* o, renderableType i, Vec2D loc, Vec2D dir, bool collides, bool draw) : Base(index, o, loc, dir, collides, draw)
{
	mesh = CopyMesh(i, (o? getGameType()->teams[o->team].color:Color()));	//each object's mesh will have an individual color, so each has its own copy
	if(collidable)
	{
		bb = new BoundingBox;
		bb->Setup(loc.x - mesh->radius, loc.y + mesh->radius, mesh->radius * 2, this);
	}
	maxSpeed = 320;
	turnForce = 3.14159f/20.0f;
	fuel = 10.0f;
	vel = o->ship->vel;
	mass = 5;
	spent = false;
	thrustForce = 1165.0f;
	lifeTime = 10.0f;
}

Missile::~Missile(void)
{
}

void Missile::run(float deltaTime)
{
	fuel -= deltaTime;
	acc.zero();
	if(!owner->ship->weapon)	//owner's ship blew up!
		spent = true;	//still has fuel, technically, but won't reaquire, and won't follow anything
						//will still be able to trace whose it was (owner) if it happens to hit something
	if(fuel > 0 && !spent)
	{
		//steering
		Vec2D diff = owner->ship->target - loc;
		float fdiff = dir.getDirection() - diff.getDirection();
		if(fdiff < 0.0f)
			fdiff += 6.28318f;	//add a circle to it, so it's within 0 to 2PI
		if(abs(fdiff) < turnForce)
			dir = diff;
		else if(fdiff >= 3.14159f)
			dir.setDirection(dir.getDirection() + turnForce);	//turn left
		else
			dir.setDirection(dir.getDirection() - turnForce);	//turn right
		dir.limit(1.0f);
		acc.set(dir.x*thrustForce/mass * deltaTime, dir.y*thrustForce/mass * deltaTime);
		if(fdiff > 3*3.14159f/4 && fdiff < 5*3.14159/4)
			acc.limit(0.1f * deltaTime);	//turn sharper
	}
	else
		lifeTime -= deltaTime;
	vel += acc;
	vel.limit(maxSpeed);
	loc += vel * deltaTime;
	bb->Update(loc.x - mesh->radius, loc.y + mesh->radius);

	if(lifeTime <= 0.0f)
		Kill();
}
