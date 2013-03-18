#ifndef QUADTREE_H
#define QUADTREE_H

#include "quadtreenode.h"

struct BoundingBox;

class Game;
class Base;

class QuadTree
{
public:
	QuadTree(void);
	QuadTree(float length, int subLevels);
	~QuadTree(void);

	void init(float length, int subLevels);
	void build(quadtreenode* node, Vec2D topleft, float length, int levels, int curLevel = 0);
	void clean(quadtreenode* node);
	void Run();
	bool inBounds(BoundingBox* a, quadtreenode* node);
	void Render();
	void Render(quadtreenode* node);
	float Distance(Base*, Base*);
	void Insert(BoundingBox* bb);
	bool Insert(BoundingBox* bb, quadtreenode* qtn);
	void UpdatePosition(BoundingBox* bb, quadtreenode* qtn);
	void PushUp(BoundingBox* bb, quadtreenode* qtn);
	void PushDown(BoundingBox* bb, quadtreenode* qtn);
	void Check(BoundingBox* bb);
	void FindSupObjs(quadtreenode* qtn, list<BoundingBox*>& supObjs);
	void FineCollisionDetection(BoundingBox* a, list<BoundingBox*>& b);
	void Remove(BoundingBox* bb);
	int Count();
	int Count(quadtreenode* qtn);

	Game* thegame;
	quadtreenode* root;
	bool entered;
	unsigned int levels;
};

#endif