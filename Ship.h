#ifndef SHIP_H
#define SHIP_H

#include "weapon.h"

class Ship : public Base
{
public:
	Ship(unsigned int index, Player* o, renderableType i, Vec2D location, Vec2D direction, float maxspeed, float sensors, int hitpoints, int weight, bool collides = true, bool draw = true);
	virtual ~Ship(void);
	int hitpoints;
	int maxHitpoints;
	float theta;    // directional angle
	//float thrustForce;
	float maxSpeed;
	Vec2D diff;	//this is for AI and should be deleted after this semester is over (AI needs to be deleted too)

	float sensorStrength;
	int mass;
	float spawnTimer;
	Weapon* weapon;
  
	// default states
	bool thrusting;
	Vec2D target;
	Base* targetObj;

	virtual void run(float deltaTime);
	virtual void Kill();
	void die();
	void slow();
	void thrust();
	void turnLeft();
	void turnRight();
};

#endif