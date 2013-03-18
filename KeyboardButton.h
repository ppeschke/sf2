#ifndef	KEYBOARDBUTTON_H
#define KEYBOARDBUTTON_H

#include "Vec2D.h"

class KeyboardButton
{
public:
	KeyboardButton(void);
	~KeyboardButton(void);

	void ResetEvent();

	Vec2D coords;
	bool hitStart;
	bool downState;
	bool hitEvent;
};

#endif