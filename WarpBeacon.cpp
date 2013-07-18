#include "Global.h"
#include "WarpBeacon.h"
#include "Player.h"
#include "Team.h"


WarpBeacon::WarpBeacon(unsigned int index, Player* p, Vec2D location, Vec2D direction, float time, bool collides, bool draw) : Base(index, p, location, direction, collides, draw)
{
	getGameType()->teams[owner->team].wb = this;
	mesh = CopyMesh(warp, getGameType()->teams[owner->team].color);
	timer = time;
}


WarpBeacon::~WarpBeacon(void)
{
}


void WarpBeacon::run(float deltaTime)
{
	if(timer > 0.0f)
		timer -= deltaTime;
	else
		Kill();
}

void WarpBeacon::Kill()
{
	dead = true;
	if(getGameType()->teams[owner->team].wb == this)
	{
		getGameType()->teams[owner->team].wb = NULL;
	}
}