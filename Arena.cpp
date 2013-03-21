#include "Arena.h"
#include "Global.h"

Arena::Arena(unsigned int index, float rad) : Base(index, NULL, Vec2D(0.0f, 0.0f), Vec2D(0.0f, 1.0f), false)
{
	radius = rad;
	mesh = CreateArena(radius, Color(1.0f, 0.4f, 0.0f));
}


Arena::~Arena(void)
{
}


void Arena::run(float deltaTime)
{
}