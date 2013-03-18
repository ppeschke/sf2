#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

class Base;
struct quadtreenode;

struct BoundingBox
{
	float x, y, length, _x, _y;
	Base* owner;
	quadtreenode* myNode;

	void Setup(float __x, float __y, float l, Base* o)
	{
		x = __x;
		y = __y;
		length = l;
		_x = x + length;
		_y = y - length;
		owner = o;
	}

	void Update(float __x, float __y)
	{
		x = __x;
		y = __y;
		_x = x + length;
		_y = y - length;
	}
};

#endif