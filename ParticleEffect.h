#ifndef PARTICLEEFFECT_H
#define PARTICLEEFFECT_H

#include "Base.h"
#include "Renderable.h"
#include "Particle.h"
#include "Texture.h"

class ParticleEffect : public Base, public Renderable
{
public:
	ParticleEffect(unsigned int index, Player* o, renderableType i, Vec2D location, Vec2D direction, Base* follow = NULL, bool collides = false, bool draw = true);
	virtual ~ParticleEffect(void);

	virtual void run(float deltaTime) = 0;
	virtual void Render(Vec2D* loc, Vec2D* dir) = 0;
	void toggle();

	bool on;

	Vec2D coords;	//coords and follow are used to place the effect either statically
	Base* follow;	//or dynamically, following an object
					//if following an object, the coords can be used to place it in relative space to the object

	int particlecount;
	Particle* particles;

	Texture* texture;
};

#endif