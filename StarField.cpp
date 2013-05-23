#include "StarField.h"
#include "Global.h"
#include "Player.h"
#include "Ship.h"


StarField::StarField(unsigned int index, Player* o, renderableType i, Vec2D location, Vec2D direction, Base* follow, bool collides, bool draw) : ParticleEffect(index, o, i, location, direction, follow, collides, draw)
{
	this->on = true;
	particlecount = 20;
	midcount = 15;
	nearcount = 10;
	particles = new Particle[particlecount];
	midField = new Particle[midcount];
	nearField = new Particle[nearcount];
	texture = (Texture*)(getGame()->getRenderable(star));	//texture is used because mesh must point to self
	get3DScreenEdges(topLeft, bottomRight);
	pcLastPos = getGame()->pc->ship->loc;
	for(int i = 0; i < particlecount; ++i)
	{
		//				Particle(radius, lifetime)
		particles[i] = Particle(4.0f, 100);
		particles[i].reset_particle(&(pcLastPos + Vec2D(randomFloat(topLeft.x, bottomRight.x), randomFloat(bottomRight.y, topLeft.y))), &Vec2D(), &Vec2D(), 0);
	}
	for(int i = 0; i < midcount; ++i)
	{
		midField[i] = Particle(4.0f, 100);
		midField[i].reset_particle(&(pcLastPos + Vec2D(randomFloat(topLeft.x, bottomRight.x), randomFloat(bottomRight.y, topLeft.y))), &Vec2D(), &Vec2D(), 0);
	}
	for(int i = 0; i < nearcount; ++i)
	{
		nearField[i] = Particle(4.0f, 100);
		nearField[i].reset_particle(&(pcLastPos + Vec2D(randomFloat(topLeft.x, bottomRight.x), randomFloat(bottomRight.y, topLeft.y))), &Vec2D(), &Vec2D(), 0);
	}
}


StarField::~StarField(void)
{
	if(particles)
		delete [] particles;
	if(midField)
		delete [] midField;
	if(nearField)
		delete [] nearField;
}

void StarField::run(float deltaTime)
{
}

void StarField::endStep(float deltaTime)
{
	Vec2D dimensions(bottomRight.x - topLeft.x, topLeft.y - bottomRight.y);
	Vec2D diff = getGame()->pc->ship->loc - pcLastPos;
	pcLastPos = getGame()->pc->ship->loc;
	for(int i = 0; i < particlecount; ++i)
	{
		particles[i].position.x += diff.x;
		particles[i].position.y += diff.y;
	}
	for(int i = 0; i < midcount; ++i)
	{
		midField[i].position.x -= diff.x/2.0f;
		midField[i].position.y -= diff.y/2.0f;
		//simple wrap
		if(midField[i].position.x < pcLastPos.x + topLeft.x)
			midField[i].position.x += dimensions.x;
		else if(midField[i].position.x > pcLastPos.x + bottomRight.x)
			midField[i].position.x -= dimensions.x;
		if(midField[i].position.y < pcLastPos.y + bottomRight.y)	//if lower than
			midField[i].position.y += dimensions.y;
		else if(midField[i].position.y > pcLastPos.y + topLeft.y)
			midField[i].position.y -= dimensions.y;
	}
	for(int i = 0; i < nearcount; ++i)
	{
		midField[i].position.x -= diff.x;
		midField[i].position.y -= diff.y;
		//simple wrap
		if(nearField[i].position.x < pcLastPos.x + topLeft.x)
			nearField[i].position.x += dimensions.x;
		else if(nearField[i].position.x > pcLastPos.x + bottomRight.x)
			nearField[i].position.x -= dimensions.x;
		if(nearField[i].position.y < pcLastPos.y + bottomRight.y)	//if lower than
			nearField[i].position.y += dimensions.y;
		else if(nearField[i].position.y > pcLastPos.y + topLeft.y)
			nearField[i].position.y -= dimensions.y;
	}
}

void StarField::Render(Vec2D* loc, Vec2D* dir)
{
	DrawBlendedParticles(particles, particlecount, texture);

	DrawBlendedParticles(midField, midcount, texture);
	DrawBlendedParticles(midField, midcount, texture);

	DrawBlendedParticles(nearField, nearcount, texture);
	DrawBlendedParticles(nearField, nearcount, texture);
	DrawBlendedParticles(nearField, nearcount, texture);
    return;
}