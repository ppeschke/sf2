#ifndef HOSTILEDRONE_H
#define HOSTILEDRONE_H

#include "Base.h"
class Ship;
class Weapon;

class hostileDrone : public Base
{
public:
	hostileDrone(unsigned int index, Player* o, renderableType mesh, Vec2D loc, Vec2D dir, Ship* chase, bool collides = true, bool draw = true);
	virtual ~hostileDrone(void);

	virtual void run(float deltaTime);
	virtual void endStep(float deltaTime);
	virtual void Kill();

	float turnForce;
	int chaseSpeed;
	int controlSpeed;
	int controlRange;		//how far out the drone will get before beginning to chase again
	int matchRange;
	float speedSmoother;	//moves between chaseSpeed (full speed) and controlSpeed (about the velocity of the ship you're following)
	Ship* flyAround;
	Vec2D* flightTarget;	//I want the drone to either chase the ship, or fly around it, so this will be set to the target ship's loc or circle
	Vec2D originalLocation;
	Weapon* weapon;
};

#endif