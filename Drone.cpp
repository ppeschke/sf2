#include "Drone.h"
#include "Mesh.h"
#include "Player.h"
#include "Ship.h"
#include "Global.h"
#include "Team.h"
#include <sstream>
using namespace std;

Drone::Drone(unsigned int index, Player* o, renderableType i, Vec2D loc, Vec2D dir, bool collides, bool draw) : Base(index, o, loc, dir, collides, draw)
{
	mesh = CopyMesh(i, (o? getGameType()->teams[o->team].color:Color()));	//each object's mesh will have an individual color, so each has its own copy
	if(collidable)
	{
		bb = new BoundingBox;
		bb->Setup(loc.x - mesh->radius, loc.y + mesh->radius, mesh->radius * 2, this);
	}
	turnForce = 3.14159f/20.0f;
	chaseSpeed = 399;//9;
	controlSpeed = 88;//2;
	speedSmoother = (float)controlSpeed;
	controlRange = 100;
	matchRange = 300;
	if(owner->ship->targetObj)
		flyAround = (Ship*)(owner->ship->targetObj);
	else
		flyAround = owner->ship;

	theta = 0;
	circle.x = cos(theta) * 10;
	circle.y = sin(theta) * 10;
	flightTarget = &circle;
	ROF = 3.0f;
}

Drone::~Drone(void)
{
}

void Drone::run(float deltaTime)
{
}

void Drone::endStep(float deltaTime)
{
	theta += 0.05f;
	circle.x = cos(theta) * flyAround->mesh->radius;
	circle.y = sin(theta) * flyAround->mesh->radius;

	circle += flyAround->loc;

	//acc.zero();	//not needed because of acc.set
	if(owner->ship->targetObj != NULL)
		flyAround = (Ship*)(owner->ship->targetObj);
	else
		flyAround = owner->ship;

	Vec2D diff = *flightTarget - loc;
	float fdiff = dir.getDirection() - diff.getDirection();
	if(fdiff < 0.0f)
		fdiff += 6.28318f;	//add a circle to it, so it's within 0 to 2PI
	if(abs(fdiff) < turnForce)
		dir = diff;
	else if(fdiff >= 3.14159f)
		dir.setDirection(dir.getDirection() + turnForce);	//turn left
	else
		dir.setDirection(dir.getDirection() - turnForce);	//turn right
	dir.normalize();
	acc.set(dir.x*chaseSpeed/5.0f * deltaTime, dir.y*chaseSpeed/5.0f * deltaTime);	//chaseSpeed is the fasteset you'll ever go
	//(we're assuming that mass of a drone is 5, because why not?
	if(fdiff > 3*3.14159f/4 && fdiff < 5*3.14159/4)
		acc.limit(4.4f * deltaTime);	//turn sharper

	vel += acc;
	//decide how to limit velocity
	if(Distance(this->loc, flyAround->loc) < controlRange)	//inside controlRange
	{
		flightTarget = &circle;
		controlSpeed = flyAround->vel.getMag() + 44.0f;	//a little faster than the target
		if(speedSmoother > controlSpeed)
			speedSmoother -= 4.4f;				//speed loss amount
		
		if(ROF <= 0.0f && flyAround != NULL && flyAround->hitpoints < flyAround->maxHitpoints)
			flyAround->hitpoints += 1;
	}
	else if(Distance(this->loc, flyAround->loc) < matchRange)	//inside matchRange
	{
		flightTarget = &circle;
		controlSpeed = flyAround->vel.getMag() + 44.0f;	//try to slowly approach control range by flying a little faster than target (flyAround)
		if(speedSmoother > controlSpeed)
			speedSmoother -= 4.4f;
		else
			speedSmoother += 4.4f;
	}
	else
	{
		flightTarget = &flyAround->loc;
		if(speedSmoother < chaseSpeed)
			speedSmoother += 44.0f;
	}
	vel.limit(speedSmoother);
	loc += vel * deltaTime;
	bb->Update(loc.x - mesh->radius, loc.y + mesh->radius);
	if(ROF <= 0)
		ROF = 3.0f;
	ROF -= deltaTime;
}
