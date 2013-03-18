#ifndef SPAWNPOINT_H
#define SPAWNPOINT_H

#include "Base.h"

class SpawnPoint : public Base
{
public:
	SpawnPoint(unsigned int index, unsigned int teamNum, Player* o, renderableType mesh, Vec2D loc, Vec2D dir, bool collides = true, bool draw = true);
	virtual ~SpawnPoint(void);
	unsigned int teamNumber;

	virtual void run();

};

#endif