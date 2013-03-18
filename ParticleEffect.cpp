#include "ParticleEffect.h"
#include "Ship.h"
#include "Missile.h"
#include "Global.h"


ParticleEffect::ParticleEffect(unsigned int index, Player* o, renderableType i, Vec2D location, Vec2D direction, Base* following, bool collides, bool draw) : Base(index, o, location, direction, collides, draw), Renderable()
{
	//particle effects don't have bounding boxes
	mesh = getGame()->getRenderable(i);
	follow = following;

	if(!follow)
		this->loc = location;

	coords = location;
	mesh = this;	//mesh->Render gets called and so mesh needs to point to me
}


ParticleEffect::~ParticleEffect(void)
{
	mesh = NULL;
}

/*void ParticleEffect::Render(Vec2D* loc, Vec2D* dir)
{
	DrawBlendedParticles(particles, particlecount, texture);
    return;
}*/

/*void ParticleEffect::run()
{
	// calculate elapsed time
	static DWORD starting_point = GetTickCount(), time;
    time = GetTickCount() - starting_point;
    starting_point = GetTickCount();

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
}*/

void ParticleEffect::toggle()
{
	on = !on;
}