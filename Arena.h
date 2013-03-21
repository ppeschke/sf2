#ifndef ARENA_H
#define ARENA_H
#include "Base.h"

class Arena : public Base
{
public:
	Arena(unsigned int index, float radius);
	virtual ~Arena(void);

	//kill is not important because the arena doesn't have any mounts
	virtual void run(float deltaTime);

	float radius;
};

#endif