#include "Turret.h"
#include "Global.h"
#include "Team.h"
#include "Player.h"

Turret::Turret(unsigned int index, Player* o, renderableType i, Base* follow, Vec2D loc, Vec2D dir, unsigned int Rof, bool collides, bool draw) : Weapon(index, o, follow, loc, dir, Rof, collides, draw)
{
	mesh = CopyMesh(i, (o? getGameType()->teams[o->team].color:Color()));	//each object's mesh will have an individual color, so each has its own copy
	if(collidable)
	{
		bb = new BoundingBox;
		bb->Setup(loc.x - mesh->radius, loc.y + mesh->radius, mesh->radius * 2, this);
	}

}

Turret::~Turret(void)
{
}

void Turret::shoot()
{
}

void Turret::run()
{
}
