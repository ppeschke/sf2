#include "EMPBomb.h"
#include "Global.h"
#include "GameType.h"
#include "Player.h"
#include "Team.h"
#include "Ship.h"


EMPBomb::EMPBomb(unsigned int index, Player* player, Vec2D location, Vec2D direction, float life, bool collides, bool draw) : Base(index, player, location, direction, collides, draw)
{
	mesh = CopyMesh(empbomb, (player? getGameType()->teams[player->team].color:Color()));	//each object's mesh will have an individual color, so each has its own copy
	if(collidable)
	{
		bb = new BoundingBox;
		bb->Setup(loc.x - mesh->radius, loc.y + mesh->radius, mesh->radius * 2, this);
	}

	lifetime = life;
}

EMPBomb::~EMPBomb(void)
{
}

void EMPBomb::run(float deltaTime)
{
	lifetime -= deltaTime;
	if(lifetime <= 0.0f)
		Kill();
}

void EMPBomb::Kill()
{
	if(owner->ship->EndSpecialAbility != NULL)
		owner->ship->EndSpecialAbility(owner->ship);
	dead = true;
}