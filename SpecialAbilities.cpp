#include "Global.h"
#include "Ship.h"
#include "WarpBeacon.h"
#include "CloakField.h"
#include "HealingCircle.h"
#include "EMPBomb.h"
#include "DamageCircle.h"

void SpawnWarpField(Ship* s)
{
	getGame()->messages.addMessage("Warp Beacon Placed");
	s->abilityTimer = 10.0f;
	s->abilityCooldownTimer = 10.0f;
	s->abilityROF = s->weapon->ROF;
	getGame()->insertObject(new WarpBeacon(getGame()->getNextIndex(), s->owner, s->loc, Vec2D(0.0f, 1.0f), 10.0f));
	s->abilityVelLimit = 0.0f;
}

void Cloak(Ship* s)
{
	s->abilityTimer = 10.0f;
	s->abilityCooldownTimer = 10.0f;
	s->drawMe = false;
	s->abilityVelLimit = s->maxSpeed/2.0f;
	s->abilityROF = s->weapon->ROF;
	getGame()->insertObject(new CloakField(getGame()->getNextIndex(), s->owner, s->loc, Vec2D(0.0f, 1.0f), s->abilityTimer));
}

void Uncloak(Ship* s)
{
	s->drawMe = true;
	s->abilityTimer = 0.0f;
}

void SpawnHealer(Ship* s)
{
	s->abilityTimer = 10.0f;
	s->abilityCooldownTimer = 10.0f;
	s->abilityVelLimit = s->maxSpeed;
	s->abilityROF = s->weapon->ROF;
	getGame()->insertObject(new HealingCircle(getGame()->getNextIndex(), s->owner, s->loc, Vec2D(0.0f, 1.0f), 10.0f));
}

void EndHealerSupport(Ship* s)
{
	s->abilityTimer = 0.0f;
}

void SpawnEMPBomb(Ship* s)
{
	s->abilityTimer = 10.0f;
	s->abilityCooldownTimer = 10.0f;
	s->abilityVelLimit = 0.0f;
	s->abilityROF = s->weapon->ROF;
	getGame()->insertObject(new EMPBomb(getGame()->getNextIndex(), s->owner, s->loc, Vec2D(0.0f, 1.0f), 10.0f));
}

void StartBarrage(Ship* s)
{
	s->abilityTimer = 10.0f;
	s->abilityROF = 0.5f;
	s->abilityCooldownTimer = 20.0f;
	s->abilityVelLimit = s->maxSpeed;
}

void EndBarrage(Ship* s)
{
	s->abilityTimer = 0.0f;
}

void SpawnDamager(Ship* s)
{
	s->abilityTimer = 10.0f;
	s->abilityCooldownTimer = 10.0f;
	s->abilityVelLimit = s->maxSpeed;
	s->abilityROF = s->weapon->ROF;
	getGame()->insertObject(new DamageCircle(getGame()->getNextIndex(), s->owner, s->loc, Vec2D(0.0f, 1.0f), 10.0f));
}

void EndDamageSupport(Ship* s)
{
	s->abilityTimer = 0.0f;
}