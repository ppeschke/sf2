#ifndef _LABEL_H
#define _LABEL_H

#include "Vec2D.h"

class _Label
{
public:
	_Label(string t, Vec2D c);
	~_Label(void);

	Vec2D coords;
	string text;
};

#endif