#include "CloakField.h"
#include "Renderable.h"
#include "Player.h"
#include "Ship.h"


CloakField::CloakField(unsigned int index, Player* player, Vec2D location, Vec2D direction, float time, bool collides, bool draw) : Base(index, player, location, direction, collides, draw)
{
	mesh = (Renderable*)CopyMesh(cloak, Color(1.0f, 0.0f, 0.0f));
	if(collidable)
	{
		bb = new BoundingBox;
		bb->Setup(loc.x - mesh->radius, loc.y + mesh->radius, mesh->radius * 2, this);
	}
	timer = time;
}


CloakField::~CloakField(void)
{
}


void CloakField::run(float deltaTime)
{
	if(timer > 0.0f)
		timer -= deltaTime;
	else
		Kill();
}

void CloakField::endStep(float deltaTime)
{
	loc = owner->ship->loc;
}

void CloakField::Kill()
{
	if(owner->ship->EndSpecialAbility != NULL)
		owner->ship->EndSpecialAbility(owner->ship);
	dead = true;
}