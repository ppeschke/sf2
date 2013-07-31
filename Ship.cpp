#include "Ship.h"
#include "Launcher.h"
#include "ShipExplosion.h"
#include "Player.h"
#include "Team.h"
#include "Global.h"
#include "SpawnPoint.h"
#include "Flag.h"
#include "CursorTarget.h"
#include "hostileDrone.h"
#include "Debris.h"

typedef void(*pfunc)(Ship*);
typedef void(*pfunc2)(Ship*);

Ship::Ship(unsigned int index, Player* o, renderableType i, Vec2D location, Vec2D direction, float maxspeed, float sensor, int hp, int weight, pfunc pF, pfunc2 pF2, bool collides, bool draw) : Base(index, o, location, direction, collides, draw)
{
	mesh = CopyMesh(i, (o? getGameType()->teams[o->team].color:Color()));	//each object's mesh will have an individual color, so each has its own copy
	if(collidable)
	{
		bb = new BoundingBox;
		bb->Setup(loc.x - mesh->radius, loc.y + mesh->radius, mesh->radius * 2, this);
	}

	SpecialAbility = pF;
	EndSpecialAbility = pF2;
	abilityTimer = 0.0f;
	abilityCooldownTimer = 0.0f;

	maxSpeed = maxspeed;
	mass = weight;
	theta = 3.14159f/(mass < 35? 35:mass);	//limit on how fast you can turn

	hitpoints = maxHitpoints = hp;
	hitpoints = maxHitpoints / 2;
	if(mass <= 0)
		mass = 1;
	else if(mass > 100)
		mass = 100;
	sensorStrength = sensor;

	thrusting = false;
	//shooting = thrusting = slowing = turningRight = turningLeft = stop = breakLock = haxors = false;

	weapon = NULL;
	targetObj = NULL;
	acc.zero();
	vel.zero();
	
	Mesh* m = (Mesh*)mesh;	//convert mesh from Renderable to Mesh pointer
	Game* g = getGame();
	bool handledHere = false;
	Base* p = NULL;
	for(unsigned int i = 0; i < m->mountNum; ++i)
	{
		if(m->mountNames[i] == "missile")
		{
			p = new Launcher(getGame()->getNextIndex(), owner, none, this, m->mountLocs[i], Vec2D(), 1.0f, shootMissile);
			handledHere = true;
		}
		else if(m->mountNames[i] == "projectile")
		{
			p = new Launcher(getGame()->getNextIndex(), owner, none, this, m->mountLocs[i], Vec2D(), 1.0f, shootProjectile);
			handledHere = true;
		}
		else if(m->mountNames[i] == "drone")
		{
			p = new Launcher(getGame()->getNextIndex(), owner, none, this, m->mountLocs[i], Vec2D(), 1.0f, shootDrone);
			handledHere = true;
		}
		if(handledHere)
		{
			g->insertObject(p);
			m->actualMounts[i] = p;
			this->weapon = ((Weapon*)p);
			handledHere = false;
		}
	}
}

Ship::~Ship(void)
{
}

void Ship::run(float deltaTime) 
{
	if(Distance(this->loc, Vec2D()) > getGame()->arena->radius && GetTickCount()%100 == 0)
	{
		Vec2D projection = loc;
		projection.scale(1.5f);
		getGame()->insertObject(new hostileDrone(getGame()->getNextIndex(), NULL, _hostileDrone, projection, Vec2D(0.0f, 1.0f), this));
	}
	InputData* id = &getGame()->inputdata;

	//special abilitiy timers
	if(abilityTimer > 0.0f)
	{
		abilityTimer -= deltaTime;
		if(abilityTimer <= 0.0f && EndSpecialAbility != NULL)	//ability just ended its time
			EndSpecialAbility(this);
	}
	else if(abilityCooldownTimer > 0.0f)
		abilityCooldownTimer -= deltaTime;
	if(hitpoints <= 0)
	{
		getGame()->debug.AddEvent("Ship died");
		die();
		//explosion
		getGame()->insertObject(new ShipExplosion(getGame()->getNextIndex(), this->loc, (int)this->mesh->radius));
		//debris
		getGame()->insertObject(new Debris(this, this->owner, this->loc, this->dir, false, true));
	}

	if(this->owner == getGame()->pc)
	{
		if(!dead)
		{
			if(id->up.downState || id->w.downState)
			{
				if(SpecialAbility == &Cloak && abilityTimer > 0.0f)
					thrusting = false;
				else
					thrusting = true;	//for turning on thruster particle effects
				thrust(deltaTime);
			}
			else
			{
				thrusting = false;
				acc.zero();  // resec accel to 0
			}
			if(id->down.downState || id->s.downState || id->r.downState)
				slow(deltaTime);
			if(id->left.downState || id->a.downState)
				turnLeft(deltaTime);
			if(id->right.downState || id->d.downState)
				turnRight(deltaTime);
			if(id->enter.hitEvent)
			{
				if(abilityCooldownTimer <= 0.0f && SpecialAbility != NULL)
					SpecialAbility(this);
				else if(SpecialAbility == NULL)
					getGame()->messages.addMessage("No special ability for this ship!");
				else
				{
					if(abilityTimer > 0.0f)
						getGame()->messages.addMessage("Special ability is running!");
					else
						getGame()->messages.addMessage("Special ability is cooling down!");
				}
				id->enter.ResetEvent();
			}
		}
	}

	vel += acc;
	if(abilityTimer > 0.0f)	//special ability is active
		vel.limit(abilityVelLimit);
	else
		vel.limit(maxSpeed);

	//if(this->owner == getGame()->pc)
	//{
	//	if(id->h.downState)	//devhax
	//	{
	//		vel.zero();
	//	}
	//}
	loc += vel * deltaTime;
	
	bb->Update(loc.x - mesh->radius, loc.y + mesh->radius);
	if(targetObj)
		target = targetObj->loc;

	if(this->owner == getGame()->pc)
	{
		if ((id->space.downState || id->mouse.left.downState) && weapon)
			weapon->shoot();
		if(id->y.downState)
			targetObj = NULL;
		if(!targetObj && getGame()->cursortarget)
			target = getGame()->cursortarget->loc;
	}
}

void Ship::die() 
{
	Ship* temp;
	//changing owner's ship from this to new ship (pod)
	temp = getGameType()->RespawnShip(this->loc, this->dir, this->vel, pod, this->owner->number);

	if(abilityTimer)
		EndSpecialAbility(this);
	Kill();
}

void Ship::Kill()
{
	dead = true;
	Mesh* m = (Mesh*)(this->mesh);
	for(unsigned int i = 0; i < m->mountNum; ++i)
		m->actualMounts[i]->Kill();

	if(getGame()->pc->ship->targetObj == this)
		getGame()->pc->ship->targetObj = NULL;	//break the lock
	unsigned int last = getGame()->lastIndex;
	for(unsigned int i = 0; i <= last; ++i)
	{
		if(getGame()->objects[i] != NULL && typeid(hostileDrone) == typeid(*(getGame()->objects[i])) && ((hostileDrone*)getGame()->objects[i])->flyAround == this)
			((hostileDrone*)getGame()->objects[i])->flyAround = NULL;
	}
}

void Ship::slow(float deltaTime)
{
	vel *= 1-((1 -  (mass * 0.01f)) * deltaTime);
}

void Ship::thrust(float deltaTime)
{
	acc.set(dir.x * maxSpeed/(mass/10.0f) * deltaTime, dir.y * maxSpeed/(mass/10.0f) * deltaTime);
}

void Ship::turnLeft(float deltaTime)
{
	//for right now, I don't want turning to be based on time... but just in case, I have deltaTime sent to the function
	dir.setDirection(dir.getDirection() + theta);
}

void Ship::turnRight(float deltaTime)
{
	dir.setDirection(dir.getDirection() - theta);
}