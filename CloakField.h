#ifndef CLOAKFIELD_H
#define CLOAKFIELD_H

#include "Base.h"

class CloakField : public Base
{
public:
	CloakField(unsigned int index, Player* player, Vec2D location, Vec2D direction, float time, bool collides = true, bool draw = false);
	virtual ~CloakField(void);

	virtual void run(float deltaTime);
	virtual void endStep(float deltaTime);

	virtual void Kill();

	float timer;
};

#endif