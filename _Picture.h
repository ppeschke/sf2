#ifndef _PICTURE_H
#define _PICTURE_H

#include "Vec2D.h"
class Sprite;

class _Picture
{
public:
	_Picture(Sprite* s, Vec2D v);
	~_Picture(void);

	Vec2D coords;

	Sprite* sprite;
};

#endif