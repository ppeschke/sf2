#include "Thruster.h"
#include "Ship.h"
#include "Missile.h"
#include "Global.h"

Thruster::Thruster(unsigned int index, Player* o, renderableType i, Vec2D location, Vec2D direction, Base* following, bool collides, bool draw) : ParticleEffect(index, o, i, location, direction, following, collides, draw)
{
	on = false;
	mesh = this;	//mesh->Render gets called and so mesh needs to point to me
	particlecount = 1000;
	particles = new Particle[particlecount];
	texture = (Texture*)(getGame()->getRenderable(orangeCircle));	//texture is used because mesh must point to self
	for(int i = 0; i < particlecount; ++i)
	{
		//				Particle(radius, lifetime)
		particles[i] = Particle(20.0f, randomNumber(1,5));
	}
}


Thruster::~Thruster(void)
{
	if(particles)
		delete [] particles;
}

void Thruster::Render(Vec2D* loc, Vec2D* dir)
{
	DrawBlendedParticles(particles, particlecount, texture);
    return;
}

void Thruster::run(float deltaTime)
{
}

void Thruster::endStep(float deltaTime)
{
	//THIS FUNCTION ISN'T USING DELTATIME YET
	if(follow)
	{
		Vec2D temp = coords;
		if(typeid(*follow) == typeid(Ship))
		{
			this->on = ((Ship*)follow)->thrusting;
		}
		else if(typeid(*follow) == typeid(Missile))
		{
			this->on = ((Missile*)follow)->fuel > 0 && !((Missile*)follow)->spent;
		}
		temp.setDirection(follow->dir.getDirection() - 3.14159f/2.0f + temp.getDirection());
		this->loc = follow->loc + temp;
	}

	Vec2D vel;
	Vec2D acc;

	if(on)
	{
		for(int index = 0; index < particlecount; ++index)
		{
			if(!particles[index].active)    // if the particle is inactive...
			{
				particles[index].active = true;    // ...activate it
				vel.zero();
				acc.zero();
				particles[index].reset_particle(&this->loc, &vel, &acc, randomNumber(0, 5));
				break;    // then stop
			}
		}
	}
	for(int index = 0; index < particlecount; ++index)
	{
		if(particles[index].active)
			particles[index].run_particle();
		if(particles[index].age > particles[index].lifespan)
		{
			//handle particle death here
			particles[index].active = false;	//just deactivate
		}
	}
}