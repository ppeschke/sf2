#include "SpawnPoint.h"
#include "Global.h"
#include "Team.h"

SpawnPoint::SpawnPoint(unsigned int index, unsigned int teamNum, Player* o, renderableType i, Vec2D loc, Vec2D dir, bool collides, bool draw) : Base(index, o, loc, dir, collides, draw)
{
	teamNumber = teamNum;
	mesh = CopyMesh(i, getGameType()->teams[teamNumber].color);	//each object's mesh will have an individual color, so each has its own copy
	if(collidable)
	{
		bb = new BoundingBox;
		bb->Setup(loc.x - mesh->radius, loc.y + mesh->radius, mesh->radius * 2, this);
	}
}

SpawnPoint::~SpawnPoint(void)
{
}


void SpawnPoint::run(float deltaTime)
{
	//note: spawn points should never move, ergo their bounding boxes never need to be updated.
}
