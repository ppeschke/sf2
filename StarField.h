#ifndef STARFIELD_H
#define STARFIELD_H

#include "ParticleEffect.h"

class StarField : public ParticleEffect
{
public:
	StarField(unsigned int index, Player* o, renderableType i, Vec2D location, Vec2D direction, Base* follow = NULL, bool collides = false, bool draw = true);
	virtual ~StarField(void);

	virtual void run(float deltaTime);
	virtual void endStep(float deltaTime);
	virtual void Render(Vec2D* loc, Vec2D* dir);

	Vec2D pcLastPos;
	Vec2D topLeft, bottomRight;	//keep these so that we can keep track of where the edges of the screen are
	int midcount;
	int nearcount;
	Particle* midField;
	Particle* nearField;
};

#endif