#ifndef _DYNAMICNUMBER_H
#define _DYNAMICNUMBER_H

#include "Vec2D.h"

class _DynamicNumber
{
public:
	_DynamicNumber(unsigned int* n, Vec2D v, bool big = false);
	~_DynamicNumber(void);

	unsigned int* num;
	Vec2D coords;
	bool big;

};

#endif