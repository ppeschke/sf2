#ifndef OBJECTIVEINDICATOR_H
#define OBJECTIVEINDICATOR_H

#include "Base.h"

class ObjectiveIndicator : public Base
{
public:
	ObjectiveIndicator(unsigned int index, Player* o, renderableType i, Vec2D location, Vec2D direction, bool myFlag);
	virtual ~ObjectiveIndicator(void);

	virtual void run();

	bool myFlag;
	Renderable* otherMesh;
	unsigned int Timer;
	bool warning;
	bool switched;
};

#endif