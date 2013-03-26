#ifndef THRUSTER_H
#define THRUSTER_H

#include "ParticleEffect.h"

class Thruster : public ParticleEffect
{
public:
	Thruster(unsigned int index, Player* o, renderableType i, Vec2D location, Vec2D direction, Base* follow = NULL, bool collides = false, bool draw = true);
	virtual ~Thruster(void);

	virtual void Render(Vec2D* loc, Vec2D* dir);
	virtual void run(float deltaTime);
	virtual void endStep(float deltaTime);
};

#endif