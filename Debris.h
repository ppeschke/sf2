#ifndef DEBRIS_H
#define DEBRIS_H

#include<list>
using namespace std;

#include "Ship.h"
#include "DebrisPart.h"
#include "ParticleEffect.h"


class Debris : public ParticleEffect
{
public:
	Debris(Ship* s, Player*, Vec2D location, Vec2D direction, bool collides = false, bool draw = true);
	virtual ~Debris(void);

	virtual void run(float deltaTime);
	virtual void Render(Vec2D* loc, Vec2D* dir);

	list<DebrisPart*> parts;
	Vec2D shipFinalDir;
};

#endif