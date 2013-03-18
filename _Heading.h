#ifndef _HEADING_H
#define _HEADING_H

#include "Vec2D.h"

class _Heading
{
public:
	_Heading(string t, Vec2D c);
	~_Heading(void);

	Vec2D coords;
	string text;
};

#endif