#include "HealingCircle.h"
#include "Global.h"
#include "Player.h"
#include "Team.h"
#include "Ship.h"


HealingCircle::HealingCircle(unsigned int index, Player* player, Vec2D location, Vec2D direction, float life, bool collides, bool draw) : Base(index, player, location, direction, collides, draw)
{
	mesh = CopyMesh(heal, (player? getGameType()->teams[player->team].color:Color()));	//each object's mesh will have an individual color, so each has its own copy
	if(collidable)
	{
		bb = new BoundingBox;
		bb->Setup(loc.x - mesh->radius, loc.y + mesh->radius, mesh->radius * 2, this);
	}
	lifetime = life;
	hitpoints = 100;
}


HealingCircle::~HealingCircle(void)
{
}

void HealingCircle::run(float deltaTime)
{
}

void HealingCircle::endStep(float deltaTime)
{
	lifetime -= deltaTime;
	if(lifetime <= 0.0f || hitpoints == 0 || owner->ship->abilityTimer <= 0.0f || owner->ship->dead)
	{
		Kill();
	}
}

void HealingCircle::Kill()
{
	if(owner->ship->EndSpecialAbility != NULL)
		owner->ship->EndSpecialAbility(owner->ship);
	dead = true;
}