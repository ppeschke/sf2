#include "Flag.h"
#include "Global.h"
#include "Team.h"

Flag::Flag(unsigned int index, renderableType i, Vec2D location, unsigned int teamNum, bool collides, bool draw) : Base(index, NULL, location, Vec2D(0.0f, 1.0f), collides, draw)
{
	teamNumber = teamNum;
	mesh = CopyMesh(i, getGameType()->teams[teamNumber].color);
	if(collidable)
	{
		bb = new BoundingBox;
		bb->Setup(loc.x - mesh->radius, loc.y + mesh->radius, mesh->radius * 2, this);
	}
}


Flag::~Flag(void)
{
}

void Flag::Kill()
{
	dead = true;
	Mesh* m = (Mesh*)(this->mesh);
	for(unsigned int i = 0; i < m->mountNum; ++i)
		m->actualMounts[i]->Kill();
	getGameType()->teams[teamNumber].flag = NULL;
}

void Flag::run()
{
}