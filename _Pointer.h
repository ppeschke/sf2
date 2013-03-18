#ifndef _POINTER_H
#define _POINTER_H

#include "Vec2D.h"
class Sprite;

class _Pointer
{
public:
	_Pointer(Sprite* s);
	~_Pointer(void);

	void KeepInBounds();

	Vec2D coords;
	Vec2D clickCoords;
	bool clickRecorded;
	Sprite* sprite;
};

#endif