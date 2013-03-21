#include "hostileDrone.h"
#include "Global.h"
#include "Ship.h"
#include "Launcher.h"

hostileDrone::hostileDrone(unsigned int index, Player* o, renderableType _mesh, Vec2D loc, Vec2D dir, Ship* _chase, bool collides, bool draw) : Base(index, o, loc, dir, collides, draw)
{
	originalLocation = loc;
	flyAround = _chase;
	flightTarget = &flyAround->loc;
	mesh = CopyMesh(_mesh, Color(1.0f, 0.4f, 0.0f));
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

	Mesh* m = ((Mesh*)mesh);
	Base* p = new Launcher(getGame()->getNextIndex(), owner, none, this, m->mountLocs[0], Vec2D(), 80, shootProjectile);
	getGame()->insertObject(p);
	m->actualMounts[0] = p;
	this->weapon = ((Weapon*)p);
}


hostileDrone::~hostileDrone(void)
{
}

void hostileDrone::run(float deltaTime)
{
	static bool oob = false;		//out of bounds

	if(Distance(originalLocation, loc) < 50.0f && (flyAround == NULL || Distance(Vec2D(), flyAround->loc) < getGame()->arena->radius))
		Kill();

	acc.zero();
	if(flyAround == NULL || flyAround->dead || ((Mesh*)flyAround->mesh)->type == pod || Distance(Vec2D(), flyAround->loc) < getGame()->arena->radius)
		flightTarget = &originalLocation;
	else
	{
		flightTarget = &flyAround->loc;
		oob = true;
	}

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
	if(flyAround)
	{
		if(Distance(this->loc, flyAround->loc) < controlRange)	//inside controlRange
		{
			controlSpeed = (int)(flyAround->vel.getMag() + 1);
			if(speedSmoother > controlSpeed)
				speedSmoother -= 0.1f;				//speed loss amount
			if(oob)
				weapon->shoot();
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
	}
	else
	{
		if(speedSmoother < chaseSpeed)
			speedSmoother += 0.1f;
	}
	vel.limit(speedSmoother);
	loc += vel;
}

void hostileDrone::Kill()
{
	dead = true;
	Mesh* m = (Mesh*)(this->mesh);
	for(unsigned int i = 0; i < m->mountNum; ++i)
		m->actualMounts[i]->Kill();
}