#include "ShipExplosion.h"
#include "Global.h"


ShipExplosion::ShipExplosion(unsigned int index, Vec2D loc, int radius) : ParticleEffect(index, NULL, none, loc, Vec2D(), NULL)
{
	particlecount = 200;
	smokeParticleCount = 200;
	spread = radius;

	plasmaIndex = 0;

	particles = new Particle[particlecount];
	smokeParticles = new Particle[smokeParticleCount];
	toggle();

	texture = ((Texture*)(getGame()->getRenderable(orangeCircle)));
	smokeTexture = ((Texture*)(getGame()->getRenderable(darkBlueCircle)));

	for(int i = 0; i < particlecount; ++i)
	{
		particles[i] = Particle(40.0f, 0);
	}
	for(int i = 0; i < smokeParticleCount; ++i)
	{
		smokeParticles[i] = Particle(20.0f, 0);
	}
}


ShipExplosion::~ShipExplosion(void)
{
	if(particles)
		delete [] particles;
	if(smokeParticles)
		delete [] smokeParticles;
}


void ShipExplosion::run()
{
	Vec2D placement;
	for(int p = plasmaIndex + 10; plasmaIndex < p && plasmaIndex < particlecount; ++plasmaIndex)	//activate particles
	{
		placement.setDirection(randomFloat(0.0f, 6.28318f));
		placement.setMag(randomFloat(0.0f, (float)(spread)));
		placement += this->loc;
		particles[plasmaIndex].reset_particle(&placement, &Vec2D(), &Vec2D(), randomNumber(1, 30));
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
		if(plasmaIndex < particlecount)
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

void ShipExplosion::Render(Vec2D* loc, Vec2D* dir)
{
	DrawBlendedParticles(particles, particlecount, texture);
	DrawBlendedParticles(smokeParticles, smokeParticleCount, smokeTexture);
	return;
}