#ifndef _BUTTTON_H
#define _BUTTON_H

#include "Vec2D.h"

class Sprite;

class _Button
{
public:
	_Button(string t, Vec2D c, Vec2D s, void(*a)(int), int p, Sprite* o);
	~_Button(void);

	bool Check(Vec2D click);

	Vec2D coords;
	Vec2D size;
	string text;

	void (*Action)(int);
	int param;

	Sprite* outline;
};

#endif