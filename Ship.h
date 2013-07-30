#ifndef SHIP_H
#define SHIP_H

#include "weapon.h"

class Ship : public Base
{
public:
	Ship(unsigned int index, Player* o, renderableType i, Vec2D location, Vec2D direction, float maxspeed, float sensors, int hitpoints, int weight, void (*pfunc)(Ship*), void (*pfunc2)(Ship*), bool collides = true, bool draw = true);
	virtual ~Ship(void);
	int hitpoints;
	int maxHitpoints;
	float theta;    // directional angle
	//float thrustForce;
	float maxSpeed;

	float sensorStrength;
	int mass;
	float spawnTimer;
	Weapon* weapon;

	void (*SpecialAbility)(Ship*);
	void (*EndSpecialAbility)(Ship*);
	float abilityTimer;
	float abilityCooldownTimer;
	float abilityVelLimit;
	float abilityROF;

	bool thrusting;
	Vec2D target;
	Base* targetObj;

	virtual void run(float deltaTime);
	virtual void Kill();
	void die();
	void slow(float deltaTime);
	void thrust(float deltaTime);
	void turnLeft(float deltaTime);
	void turnRight(float deltaTime);
};

#endif