#include "Indicator.h"
#include "Global.h"
#include "Team.h"
#include "Player.h"

Indicator::Indicator(unsigned int index, Player* o, renderableType i, Vec2D loc, Vec2D dir, bool collides, bool draw) : Base(index, o, loc, dir, collides, draw)
{
	mesh = CopyMesh(i, (o? getGameType()->teams[o->team].color:Color()));	//each object's mesh will have an individual color, so each has its own copy
	if(collidable)
	{
		bb = new BoundingBox;
		bb->Setup(loc.x - mesh->radius, loc.y + mesh->radius, mesh->radius * 2, this);
	}
}

Indicator::~Indicator(void)
{
}
