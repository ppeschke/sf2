#include "ObjectiveIndicator.h"
#include "Global.h"
#include "Team.h"
#include "Player.h"
#include "Ship.h"
#include "Flag.h"

ObjectiveIndicator::ObjectiveIndicator(unsigned int index, Player* o, renderableType i, Vec2D location, Vec2D direction, bool mF) : Base(index, o, location, direction, false, false)
{
	if(!getGame()->pc)
		drawMe = false;
	myFlag = mF;
	warning = false;
	switched = false;

	otherMesh = NULL;
	mesh = CopyMesh(i, Color(1.0f, 1.0f, 0.0f));	//each object's mesh will have an individual color, so each has its own copy
	if(myFlag)
	{
		otherMesh = CopyMesh(i, Color(1.0f, 0.0f, 0.0f));
		Timer = 40;
	}
}

ObjectiveIndicator::~ObjectiveIndicator(void)
{
	if(otherMesh)
		delete otherMesh;
}


void ObjectiveIndicator::run()
{
	warning = false;
	if(getGame()->pc != NULL)
	{
		drawMe = true;
		Ship* ps = getGame()->pc->ship;
		Base* f;
		if(myFlag)
		{
			f = getGameType()->teams[owner->team].flag;
			if(f == NULL)
			{
				f = getGameType()->teams[owner->team].carryingFlag;
				--Timer;
				warning = true;
			}
			if(f == NULL)		//sometimes flag will be NULL and carryingFlag will be NULL because a ship just dropped the flag (died) and the new flag isn't in the system this frame
			{
				drawMe = false;		//in that case, we just don't want to draw the indicator
			}
		}
		else
		{
			f = getGameType()->teams[getGame()->pc->team? 0:1].flag;
			if(f == NULL)
			{
				f = (Base*)getGameType()->teams[owner->team].spawnpoint;
			}
		}
		if(drawMe)
		{
			Vec2D place = (f->loc - ps->loc);
			place.normalize();
			place.scale(ps->mesh->radius + this->mesh->radius);
			loc = ps->loc + place;
			dir = loc - ps->loc;
		}
	}
	else
		drawMe = false;
	if(drawMe)
	{
		if(Timer == 0)
		{
			//reset timer and switch meshes
			Timer = 40;
			Renderable* temp = mesh;
			mesh = otherMesh;
			otherMesh = temp;
			switched = !switched;
		}
		if(!warning)
		{
			Timer = 40;
			if(switched)
			{	//switch back
				Renderable* temp = mesh;
				mesh = otherMesh;
				otherMesh = temp;
				switched = !switched;
			}
		}
	}
}