#include "_DynamicNumber.h"

_DynamicNumber::_DynamicNumber(unsigned int* n, Vec2D v, bool b)
{
	num = n;
	coords = v;
	big = b;
}

_DynamicNumber::~_DynamicNumber(void)
{
}
