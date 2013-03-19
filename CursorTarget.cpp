#include "CursorTarget.h"
#include "Renderable.h"
#include "Global.h"
#include "Ship.h"
#include "Player.h"
#include "Team.h"

CursorTarget::CursorTarget(unsigned int index, Player* o, renderableType i, Vec2D location, Vec2D direction, bool collides, bool draw) : Base(index, o, location, direction, collides, draw)
{
	mesh = CopyMesh(i, (o? getGameType()->teams[o->team].color:Color()));	//each object's mesh will have an individual color, so each has its own copy
	if(collidable)
	{
		bb = new BoundingBox;
		bb->Setup(loc.x - mesh->radius, loc.y + mesh->radius, mesh->radius * 2, this);
	}
	Game* g = getGame();
	Vec2D sl = g->pc->ship->loc;

	relCoords = this->loc - sl;

	if(Distance(sl, this->loc) > g->pc->ship->sensorStrength)
	{
		relCoords.limit(g->pc->ship->sensorStrength);
		this->loc = relCoords + sl;
	}
}


CursorTarget::~CursorTarget(void)
{
}

void CursorTarget::run(float deltaTime)
{
	Game* g = getGame();
	Vec2D sl = g->pc->ship->loc;
	relCoords += g->inputdata.mouse.coords;
	this->loc = relCoords + sl;
	if(Distance(sl, this->loc) > g->pc->ship->sensorStrength)
	{
		relCoords = loc - sl;
		relCoords.limit(g->pc->ship->sensorStrength);
		this->loc = relCoords + sl;
	}
	this->bb->Update(loc.x - mesh->radius, loc.y + mesh->radius);
}