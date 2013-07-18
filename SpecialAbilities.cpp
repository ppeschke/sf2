#include "Global.h"
#include "Ship.h"
#include "WarpBeacon.h"
#include "CloakField.h"

void SpawnWarpField(Ship* s)
{
	getGame()->messages.addMessage("Warp Beacon Placed");
	s->abilityTimer = 10.0f;
	s->abilityCooldownTimer = 10.0f;
	getGame()->insertObject(new WarpBeacon(getGame()->getNextIndex(), s->owner, s->loc, Vec2D(0.0f, 1.0f), 10.0f));
	s->abilityVelLimit = 0.0f;
}

void Cloak(Ship* s)
{
	s->abilityTimer = 10.0f;
	s->abilityCooldownTimer = 10.0f;
	s->drawMe = false;
	s->abilityVelLimit = s->maxSpeed/2.0f;
	getGame()->insertObject(new CloakField(getGame()->getNextIndex(), s->owner, s->loc, Vec2D(0.0f, 1.0f), s->abilityTimer));
}

void Uncloak(Ship* s)
{
	s->drawMe = true;
	s->abilityTimer = 0.0f;
}