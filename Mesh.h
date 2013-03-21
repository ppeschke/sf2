#ifndef MESH_H
#define MESH_H

#include "Renderable.h"
#include "Polygon.h"

class Base;

class Mesh : public Renderable
{
public:
	Mesh();
	Mesh(const char* filename, renderableType mT, int scale);
	Mesh(const Mesh& orig);
	virtual ~Mesh(void);
	Mesh& operator=(const Mesh& orig);

	virtual void Render(Vec2D* loc, Vec2D* dir);
	void Import(const char*, int scale);

	//YAY POINTERS!!!1!
	sf2::Polygon* polys;	//pointer to a dynamically allocated array of polygons

	unsigned int polyNum;
	Vec2D* mountLocs;		//mount locations for things like thrusters and weapon turrets/launchers
	string* mountNames;
	unsigned int mountNum;
	Base** actualMounts;	//needed to delete the objects dependant on the ship whenever the ship gets destroyed
};

#endif