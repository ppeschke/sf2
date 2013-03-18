#ifndef QUADTREENODE_H
#define QUADTREENODE_H

#include <list>
using namespace std;

#include "Vec2D.h"
#include "BoundingBox.h"

struct quadtreenode
{
	Vec2D bounds[4];
	quadtreenode* children[4];
	quadtreenode* parent;
	float length;
	unsigned int level;
	list<BoundingBox*> objs;
};

#endif