#include "QuadTree.h"
#include "Mesh.h"
#include "GameType.h"
#include "Game.h"
#include "Global.h"
void DrawMesh(Mesh* mesh, Vec2D* loc, Vec2D* dir);
#define NULL 0

QuadTree::QuadTree(void)
{
	root = NULL;
	entered = false;
}

QuadTree::QuadTree(float length, int sublevels)
{
	entered = false;
	init(length, sublevels);
}

void QuadTree::init(float length, int sublevels)
{
	root = new quadtreenode;
	root->parent = NULL;
	levels = sublevels + 1;
	build(root, Vec2D(0.0f - length/2.0f, length/2.0f), length, sublevels);
}

QuadTree::~QuadTree(void)
{
	if(root)
		clean(root);
}

void QuadTree::build(quadtreenode* node, Vec2D topleft, float length, int levels, int curLevel)
{
	node->level = curLevel + 1;
	node->length = length;
	node->bounds[0] = Vec2D(topleft.x, topleft.y);
	node->bounds[1] = Vec2D(topleft.x + node->length, topleft.y);
	node->bounds[2] = Vec2D(topleft.x + node->length, topleft.y - node->length);
	node->bounds[3] = Vec2D(topleft.x, topleft.y - node->length);
	if(curLevel < levels)
	{
		for(unsigned int i = 0; i < 4; ++i)
		{
			switch(i)
			{
			case 0:
				topleft = Vec2D(node->bounds[0].x, node->bounds[0].y);
				break;
			case 1:
				topleft = Vec2D(node->bounds[0].x + node->length/2.0f, node->bounds[0].y);
				break;
			case 2:
				topleft = Vec2D(node->bounds[0].x + node->length/2.0f, node->bounds[0].y - node->length/2.0f);
				break;
			case 3:
				topleft = Vec2D(node->bounds[0].x, node->bounds[0].y - node->length/2.0f);
				break;
			}
			node->children[i] = new quadtreenode;
			node->children[i]->parent = node;
			build(node->children[i], topleft, length/2.0f, levels, curLevel + 1);
		}
	}
	else
	{
		for(unsigned int i = 0; i < 4; ++i)
		{
			node->children[i] = NULL;
		}
	}
}

void QuadTree::clean(quadtreenode* node)
{
	if(node != NULL)
	{
		if(node->children[0] != NULL)
		{
			for(unsigned int i = 0; i < 4; ++i)
				clean(node->children[i]);
		}
		else
		{
			delete node;
			node = NULL;
		}
	}
}

void QuadTree::Run()
{
	thegame->debug.EnterSubProcess("Collision Run Loop", 0);
	for(unsigned int index = 0; index <= thegame->lastIndex; ++index)	//loop through each object in the game
	{
		if(thegame->objects[index] != NULL && thegame->objects[index]->collidable)
		{
			thegame->debug.EnterSubProcess(string("Checking ") + typeid(*(thegame->objects[index])).name() + " " + itos(thegame->objects[index]->index), 0);
			Check(thegame->objects[index]->bb);
			thegame->debug.ExitSubProcess();
		}
	}
	thegame->debug.ExitSubProcess();
}

bool QuadTree::inBounds(BoundingBox* a, quadtreenode* node)
{
	if(a->x > node->bounds[0].x && a->_x < node->bounds[1].x
		&& a->y < node->bounds[0].y && a->_y > node->bounds[3].y)
		return true;
	return false;
}

void QuadTree::Render()
{
	Render(root);
}

void QuadTree::Render(quadtreenode* node)
{
	/*if(node->numObjs == 0)
		DrawRect(node->bounds->x, node->bounds->y, node->length, node->length, D3DCOLOR_XRGB(255, 255, 255));
	else */if(node->objs.size() == 1)
		DrawRect(node->bounds->x, node->bounds->y, node->length, node->length, D3DCOLOR_XRGB(255, 255, 0));
	else if(node->objs.size() >= 2)
		DrawRect(node->bounds->x, node->bounds->y, node->length, node->length, D3DCOLOR_XRGB(255, 0, 0));
	if(node->children[0])
	{
		for(unsigned int i = 0; i < 4; ++i)
		{
			Render(node->children[i]);
		}
	}
}

float inline QuadTree::Distance(Base* a, Base* b)
{
	return sqrt(pow((a->loc.x - b->loc.x), 2) + pow((a->loc.y - b->loc.y), 2));
}

void QuadTree::Insert(BoundingBox* bb)
{
	if(!Insert(bb, root))
	{
		bb->myNode = root;
	}
	bb->myNode->objs.insert(bb->myNode->objs.end(), bb);	//put it into the new node
}

bool QuadTree::Insert(BoundingBox* bb, quadtreenode* qtn)
{
	bool done = false;
	if(inBounds(bb, qtn))
	{
		if(qtn->children[0])
		{
			for(int i = 0; i < 4; ++i)
			{
				done = Insert(bb, qtn->children[i]);
				if(done)
					break;
			}
		}
		if(!done)	//no children or didn't fit into any of them
		{
			bb->myNode = qtn;
			done = true;
		}
	}
	return done;
}

void QuadTree::UpdatePosition(BoundingBox* bb, quadtreenode* qtn)
{
	//remove bb from qtn
	bb->myNode->objs.remove(bb);

	if(inBounds(bb, qtn))	//if it fits in me
		PushDown(bb, qtn);	//see if it fits in children too
	else
		PushUp(bb, qtn);	//doesn't fit in me, push to parent

	bb->myNode->objs.insert(bb->myNode->objs.end(), bb);	//put it into the new node (now indicated by bb->myNode)

}

void QuadTree::PushUp(BoundingBox* bb, quadtreenode* qtn)
{
	if(qtn->parent)
	{
		//push up
		bb->myNode = qtn->parent;

		//check if parent needs a push up too
		if(!inBounds(bb, qtn->parent))
			PushUp(bb, qtn->parent);
	}
}

void QuadTree::PushDown(BoundingBox* bb, quadtreenode* qtn)
{
	//try to push down
	if(qtn->children[0])
	{
		for(int i = 0; i < 4; ++i)
		{
			if(inBounds(bb, qtn->children[i]))
			{
				bb->myNode = qtn->children[i];

				PushDown(bb, qtn->children[i]);
				break;
			}
		}
	}
}

void QuadTree::Check(BoundingBox* bb)
{
	list<BoundingBox*> supObjs;
	thegame->debug.EnterSubProcess("Finding SuperObjects", 0);
	if(bb->myNode->parent)
		FindSupObjs(bb->myNode->parent, supObjs);
	thegame->debug.ExitSubProcess();
	thegame->debug.EnterSubProcess("AddThisNode", 0);
	if(bb->myNode->objs.size() > 1)
	{
		bool found = false;
		for(list<BoundingBox*>::iterator index = bb->myNode->objs.begin(); index != bb->myNode->objs.end(); ++index)
		{
			if((*index) == bb)
			{
				found = true;
				continue;
			}
			if(found)
				supObjs.insert(supObjs.end(), *index);	//add everyone else in this qtn after me
		}
	}
	thegame->debug.ExitSubProcess();
	if(supObjs.size() > 0)
	{
		//thegame->debug.AddEvent(string("Fine collision detection for ") + itos(supObjs.size()) + " objects");
		thegame->debug.EnterSubProcess("Fine Collision Detection", 0);
		FineCollisionDetection(bb, supObjs);
		thegame->debug.ExitSubProcess();
	}
}

void QuadTree::FindSupObjs(quadtreenode* qtn, list<BoundingBox*>& supObjs)
{
	//insert into subObjs
	supObjs.insert(supObjs.end(), qtn->objs.begin(), qtn->objs.end());
	if(qtn->parent)
			FindSupObjs(qtn->parent, supObjs);
}

void QuadTree::FineCollisionDetection(BoundingBox* a, list<BoundingBox*>& b)
{
	for(list<BoundingBox*>::iterator checkIndex = b.begin(); checkIndex != b.end(); ++checkIndex)
	{
		if(sqrt(pow((a->owner->loc.x - (*checkIndex)->owner->loc.x), 2)
			+ pow((a->owner->loc.y - (*checkIndex)->owner->loc.y), 2))
			<= a->owner->mesh->radius + (*checkIndex)->owner->mesh->radius)		//within bounding circle
		{
			thegame->debug.EnterSubProcess("OnCollision", 0);
			thegame->gametype->onCollision(a->owner, (*checkIndex)->owner);
			thegame->debug.ExitSubProcess();
		}
	}
}

void QuadTree::Remove(BoundingBox* bb)
{
	bb->myNode->objs.remove(bb);
}

int QuadTree::Count()
{
	return Count(root);
}

int QuadTree::Count(quadtreenode* qtn)
{
	int c = qtn->objs.size();
	if(qtn->children[0])
	{
		for(int i = 0; i < 4; ++i)
		{
			c += Count(qtn->children[i]);
		}
	}
	return c;
}