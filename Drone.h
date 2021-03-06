#ifndef DRONE_H
#define DRONE_H

#include "Base.h"

class Ship;

class Drone : public Base
{
public:
	Drone(unsigned int index, Player* o, renderableType mesh, Vec2D loc, Vec2D dir, bool collides = true, bool draw = true);
	virtual ~Drone(void);

	virtual void run(float DeltaTime);
	virtual void endStep(float deltaTime);

	float turnForce;
	float chaseSpeed;
	float controlSpeed;
	float controlRange;		//how far out the drone will get before beginning to chase again
	float matchRange;
	float speedSmoother;	//moves between chaseSpeed (full speed) and controlSpeed (about the velocity of the ship you're following)
	Ship* flyAround;
	Vec2D circle;
	float theta;
	Vec2D* flightTarget;	//I want the drone to either chase the ship, or fly around it, so this will be set to the target ship's loc or circle
	float ROF;

};

#endif