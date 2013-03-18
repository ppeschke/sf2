#include "Global.h"
#include "MissileExplosion.h"


MissileExplosion::MissileExplosion(unsigned int index, Vec2D loc) : ParticleEffect(index, NULL, none, loc, Vec2D(), NULL)
{
	runIndex = 0;
	spread = 10;
	particlecount = 100;
	smokeParticleCount = 100;
	particles = new Particle[particlecount];
	smokeParticles = new Particle[smokeParticleCount];
	toggle();
	texture = (Texture*)(getGame()->getRenderable(orangeCircle));
	smokeTexture = (Texture*)(getGame()->getRenderable(darkBlueCircle));

	for(int i = 0; i < particlecount; ++i)
	{
		//				Particle(radius, lifetime)
		particles[i] = Particle(20.0f, 1);
	}

	for(int i = 0; i < smokeParticleCount; ++i)
		smokeParticles[i] = Particle(15.0f, 1);
}


MissileExplosion::~MissileExplosion(void)
{
	if(smokeParticles)
		delete [] smokeParticles;
}

void MissileExplosion::run()
{
	for(int p = runIndex + 10; runIndex < p && runIndex < particlecount; ++runIndex)	//activate particles
	{
		particles[runIndex].reset_particle(&Vec2D(loc.x + randomNumber(-spread, spread), loc.y + randomNumber(-spread, spread)), &Vec2D(), &Vec2D(), randomNumber(1, 10));
	}
	for(int j = 0; j < particlecount; ++j)
	{
		if(particles[j].active)
			particles[j].run_particle();

		if(particles[j].age > particles[j].lifespan && particles[j].active)
		{
			particles[j].active = false;		//deactivate particles
			//for(int m = j; m < smokeParticleCount; m += 100)
				//smokeParticles[m].reset_particle(&Vec2D(particles[j].position.x, particles[j].position.y), &Vec2D(), &Vec2D(), randomNumber(10, 500));
			smokeParticles[j].reset_particle(&Vec2D(particles[j].position.x, particles[j].position.y), &Vec2D(), &Vec2D(), randomNumber(10, 500));
		}
	}
	for(int j = 0; j < smokeParticleCount; ++j)
	{
		if(smokeParticles[j].active)
			smokeParticles[j].run_particle();
		if(smokeParticles[j].age > smokeParticles[j].lifespan)
			smokeParticles[j].active = false;
	}
	bool done = true;
	for(int index = 0; index < smokeParticleCount; ++index)
	{
		if(runIndex < particlecount)
		{
			done = false;
			break;	//haven't finished starting all the particles yet
		}
		if(smokeParticles[index].active)
			done = false;
	}
	if(done)
		dead = true;
}

void MissileExplosion::Render(Vec2D* loc, Vec2D* dir)
{
	DrawBlendedParticles(particles, particlecount, texture);
	DrawBlendedParticles(smokeParticles, smokeParticleCount, smokeTexture);
	return;
}