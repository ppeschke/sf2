#ifndef FLAG_H
#define FLAG_H

#include "Base.h"
class Ship;

class Flag : public Base
{
public:
	Flag(unsigned int index, renderableType i, Vec2D location, unsigned int teamNum, bool collides = true, bool draw = true);
	virtual ~Flag(void);

	virtual void run();
	virtual void Kill();

	unsigned int teamNumber;
};

#endif