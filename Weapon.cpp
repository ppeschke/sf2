#include "Weapon.h"

Weapon::Weapon(unsigned int index, Player* o, Base* _follow, Vec2D loc, Vec2D dir, unsigned int _ROF, bool collides, bool draw) : Base(index, o, loc, dir, collides, draw)
{
	coords = loc;
	follow = _follow;
	ROF = _ROF;
	Timer = 0;
}

Weapon::~Weapon(void)
{
}