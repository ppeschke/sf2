#ifndef MOUSEINPUT_H
#define MOUSEINPUT_H

#include "KeyboardButton.h"
#include "Vec2D.h"

class MouseInput
{
public:
	MouseInput(void);
	~MouseInput(void);

	KeyboardButton left;
	KeyboardButton right;
	KeyboardButton middle;

	Vec2D coords;
};

#endif