#ifndef MISSILEEXPLOSION_H
#define MISSILEEXPLOSION_H

#include "particleeffect.h"

class MissileExplosion : public ParticleEffect
{
public:
	MissileExplosion(unsigned int index, Vec2D loc);
	virtual ~MissileExplosion(void);

	virtual void run();
	virtual void Render(Vec2D* loc, Vec2D* dir);

	Texture* smokeTexture;
	int smokeParticleCount;
	Particle* smokeParticles;
	int spread;
	int runIndex;
};

#endif