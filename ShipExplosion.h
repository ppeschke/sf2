#ifndef SHIPEXPLOSION_H
#define SHIPEXPLOSION_H

#include "ParticleEffect.h"

class ShipExplosion : public ParticleEffect
{
public:
	ShipExplosion(unsigned int index, Vec2D loc, int radius);
	virtual ~ShipExplosion(void);

	virtual void run();
	virtual void Render(Vec2D* loc, Vec2D* dir);

	Texture* smokeTexture;
	int smokeParticleCount;
	Particle* smokeParticles;
	int spread;
	int plasmaIndex;
};

#endif