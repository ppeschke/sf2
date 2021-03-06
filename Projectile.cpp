#include "Projectile.h"
#include "Mesh.h"
#include "Ship.h"
#include "Player.h"
#include "Global.h"
#include "Team.h"

Projectile::Projectile(unsigned int index, Player* o, renderableType i, Vec2D loc, Vec2D dir, bool collides, bool draw) : Base(index, o, loc, dir, collides, draw)
{
	lifetime = 0.6f;
	mesh = CopyMesh(i, (o? getGameType()->teams[o->team].color:Color()));	//each object's mesh will have an individual color, so each has its own copy
	if(collidable)
	{
		bb = new BoundingBox;
		bb->Setup(loc.x - mesh->radius, loc.y + mesh->radius, mesh->radius * 2, this);
	}
	if(owner)
		this->vel = this->dir * 440 + owner->ship->vel;
	else
		this->vel = this->dir * 440;
}

Projectile::~Projectile(void)
{
}


void Projectile::run(float deltaTime)
{
	if(lifetime <= 0.0f)
		Kill();
	this->loc += this->vel * deltaTime;
	bb->Update(loc.x - mesh->radius, loc.y + mesh->radius);
	lifetime -= deltaTime;
}