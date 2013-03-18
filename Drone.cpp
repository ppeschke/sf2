#include "Drone.h"
#include "Mesh.h"
#include "Player.h"
#include "Ship.h"
#include "Global.h"
#include "Team.h"

Drone::Drone(unsigned int index, Player* o, renderableType i, Vec2D loc, Vec2D dir, bool collides, bool draw) : Base(index, o, loc, dir, collides, draw)
{
	mesh = CopyMesh(i, (o? getGameType()->teams[o->team].color:Color()));	//each object's mesh will have an individual color, so each has its own copy
	if(collidable)
	{
		bb = new BoundingBox;
		bb->Setup(loc.x - mesh->radius, loc.y + mesh->radius, mesh->radius * 2, this);
	}
	turnForce = 3.14159f/20.0f;
	chaseSpeed = 9;
	controlSpeed = 2;
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
	ROF = 100;
}

Drone::~Drone(void)
{
}

void Drone::run()
{
	theta += 0.05f;
	circle.x = cos(theta) * flyAround->mesh->radius;
	circle.y = sin(theta) * flyAround->mesh->radius;

	circle += flyAround->loc;

	acc.zero();
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
	dir.limit(1.0f);
	acc.add(dir.x/chaseSpeed, dir.y/chaseSpeed);	//chaseSpeed is the fasteset you'll ever go
	if(fdiff > 3*3.14159f/4 && fdiff < 5*3.14159/4)
		acc.limit(0.1f);	//turn sharper

	vel += acc;
	//decide how to limit velocity
	if(Distance(this->loc, flyAround->loc) < controlRange)	//inside controlRange
	{
		flightTarget = &circle;
		controlSpeed = (int)(flyAround->vel.getMag() + 1);
		if(speedSmoother > controlSpeed)
			speedSmoother -= 0.1f;				//speed loss amount
		
		if(ROF == 0 && flyAround != NULL && flyAround->hitpoints < flyAround->maxHitpoints)
			flyAround->hitpoints += 1;
	}
	else if(Distance(this->loc, flyAround->loc) < matchRange)	//inside matchRange
	{
		controlSpeed = (int)(flyAround->vel.getMag() + 5);	//try to slowly approach control range by flying a little faster than target (flyAround)
		if(speedSmoother > controlSpeed)
			speedSmoother -= 0.1f;
		else
			speedSmoother += 0.1f;
	}
	else
	{
		flightTarget = &flyAround->loc;
		if(speedSmoother < chaseSpeed)
			speedSmoother += 0.1f;
	}
	vel.limit(speedSmoother);
	loc += vel;
	bb->Update(loc.x - mesh->radius, loc.y + mesh->radius);
	if(ROF == 0)
		ROF = 100;
	--ROF;
}