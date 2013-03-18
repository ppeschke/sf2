#include <fstream>
#include <list>
#include <string>
#include "Mesh.h"
#include "Global.h"
using namespace std;	//this may cause trouble because we may have to refer to Polygons which is in the sf2 namespace

Mesh::Mesh()
{
	polys = NULL;
	type = none;
	polyNum = 0;
	actualMounts = NULL;
}

Mesh::Mesh(const char* filename, renderableType mT, int scale)
{
	polys = NULL;
	mountLocs = NULL;
	mountNames = NULL;
	mountNum = 0;
	type = mT;
	polyNum = 0;
	Import(filename, scale);
	actualMounts = NULL;
}

Mesh::Mesh(const Mesh& orig)
{
	//another copy constructor, good to keep up the practice
	//if we're copying onto self (myObj = myObj;)
	if(this == &orig)
	{
		return;
	}

	//copy the new
	this->type = orig.type;
	this->polyNum = orig.polyNum;
	this->radius = orig.radius;
	this->mountNum = orig.mountNum;
	if(polyNum)
	{
		this->polys = new sf2::Polygon[polyNum];
		for(unsigned int index = 0; index < polyNum; ++index)
			this->polys[index] = orig.polys[index];	//this calls the Polygon copy constructor
	}
	else
		this->polys = NULL;
	if(mountNum)
	{
		this->mountNames = new string[mountNum];
		this->mountLocs = new Vec2D[mountNum];
		for(unsigned int i = 0; i < mountNum; ++i)
		{
			this->mountLocs[i] = orig.mountLocs[i];
			this->mountNames[i] = orig.mountNames[i];
		}
	}
	else
	{
		this->mountLocs = NULL;
		this->mountNames = NULL;
	}
}

Mesh& Mesh::operator=(const Mesh& orig)
{
	if(this == &orig)
		return (*this);

	//clean the old
	if(this->polys)
	{
		delete [] this->polys;	//this will call the destructor for the Polygon class
		this->polys = NULL;
	}
	polyNum = orig.polyNum;
	type = orig.type;
	this->radius = orig.radius;
	this->mountNum = orig.mountNum;
	if(polyNum)
	{
		this->polys = new sf2::Polygon[polyNum];
		for(unsigned int i = 0; i < polyNum; ++i)
		{
			polys[i] = orig.polys[i];
		}
	}
	if(mountNum)
	{
		this->mountNames = new string[mountNum];
		this->mountLocs = new Vec2D[mountNum];
		for(unsigned int i = 0; i < mountNum; ++i)
		{
			this->mountLocs[i] = orig.mountLocs[i];
			this->mountNames[i] = orig.mountNames[i];
		}
	}
	return *this;
}

Mesh::~Mesh(void)
{
	if(polys)
		delete [] polys;
	if(mountLocs)
		delete [] mountLocs;
	if(mountNames)
		delete [] mountNames;
	if(actualMounts)
	{
		/*CHANGES - Mounts are now killed (dead = true) from the Missile kill area (GameType::onCollision) and the ~Ship()
					because ...*/
		/*if(getGame()->state != over)	//if the game is over, all objects in thegame->objects will be deleted. DO NOT try to kill them from here
		{						//because you may access a mount that has already been deleted
			for(unsigned int i = 0; i < mountNum; ++i)
			{
				if(actualMounts[i] != NULL)
					actualMounts[i]->dead = true;
			}
		}*/
		delete [] actualMounts;	//they've already been killed, don't worry about tracking them
	}	
}

void Mesh::Render(Vec2D* loc, Vec2D* dir)
{
	DrawMesh(this, loc, dir);
}

void Mesh::Import(const char* filename, int scale)
{
	enum fileImportState {looking, meshCoords, mountName, mountCoords, ended, error};
	fileImportState state = looking;
	float furthest = 0.0f, distance = 0.0f;
	sf2::CUSTOMVERTEX* vtemp = NULL;
	sf2::Polygon temp;
	string line;
	list<float> xcoords;
	list<float> ycoords;
	list<sf2::Polygon> mypolys;	//list of polygons for this mesh
	list<string> mountNamesList;
	list<Vec2D> mountCoordsList;

	float x, y;
	ifstream fin(filename);
	if(fin.fail())
	{
		exit(EXIT_FAILURE);
	}

	getline(fin, line, '\n');	//we read a line and look at what it contains (if state is looking, it should contain instructions on what is coming next in the file)
	while(state != ended && state != error)
	{
		if(fin.eof())
		{
			if(state == looking)
				state = ended;
			else
				state = error;	//expected more data, but got end of file
			continue;
		}

		switch(state)
		{
		case looking:
			if(line[0] == '#')
				break;
			else if(line == "newpoly")
				state = meshCoords;
			else if(line == "mount")
				state = mountName;
			else
				state = error;
			break;
		case meshCoords:
			if(line == "endpoly")
			{
				state = looking;
				//commit temp to mypolys
				temp.length = xcoords.size();
				temp.vertices = new sf2::CUSTOMVERTEX[temp.length];
				list<float>::iterator xindex = xcoords.begin();
				list<float>::iterator yindex = ycoords.begin();
				for(unsigned int i = 0; i < temp.length; ++xindex, ++yindex, ++i)
				{
					temp.vertices[i].X = (*xindex);
					temp.vertices[i].Y = (*yindex);
					temp.vertices[i].Z = 0.0f;
					temp.vertices[i].COLOR = D3DCOLOR_XRGB(255, 255, 255);
				}
				//add new poly
				mypolys.insert(mypolys.end(), temp);
				temp.vertices = NULL;	//sever connection (don't delete, because they exist in the heap now, but we'll make another using this variable)
				//end commit


				//empty list for new polygon
				yindex = ycoords.begin();
				for(xindex = xcoords.begin(); !xcoords.empty(); xindex = xcoords.begin(), yindex = ycoords.begin())
				{
					xcoords.erase(xindex);
					ycoords.erase(yindex);
				}
			}
			else
			{
				x = processNumber(line.substr(0, line.find_first_of(' ')));
				x *= scale;
				xcoords.insert(xcoords.end(), x);
				y = processNumber(line.substr(line.find_first_of(' ') + 1));
				y *= scale;
				ycoords.insert(ycoords.end(), y);
				distance = sqrt(pow(x, 2) + pow(y, 2));
				if(distance > furthest)
					furthest = distance;
			}
			break;
		case mountName:
			mountNamesList.insert(mountNamesList.end(), line);	//commit
			state = mountCoords;
			break;
		case mountCoords:
			state = looking;
			Vec2D tempMountCoords;
			tempMountCoords.x = processNumber(line.substr(0, line.find_first_of(' ')));
			tempMountCoords.x *= scale;
			tempMountCoords.y = processNumber(line.substr(line.find_first_of(' ') + 1));
			tempMountCoords.y *= scale;
			mountCoordsList.insert(mountCoordsList.end(), tempMountCoords);	//commit
			break;
		}	//end switch
		getline(fin, line, '\n');
	}	//end while
	fin.close();
	if(state == error)
	{
		exit(EXIT_FAILURE);
	}
	//now make room in the mesh object
	polys = new sf2::Polygon[mypolys.size()];
	int i = 0;
	for(list<sf2::Polygon>::iterator index = mypolys.begin(); index != mypolys.end(); ++index, ++i)
	{
		polys[i] = (*index);
		++polyNum;	//increment polyNum
	}
	mountLocs = new Vec2D[mountCoordsList.size()];
	mountNames = new string[mountNamesList.size()];
	mountNum = mountCoordsList.size();
	i = 0;
	for(list<Vec2D>::iterator index = mountCoordsList.begin(); index != mountCoordsList.end(); ++index)
		mountLocs[i++] = (*index);
	i = 0;
	for(list<string>::iterator index = mountNamesList.begin(); index != mountNamesList.end(); ++index)
		mountNames[i++] = (*index);
	radius = furthest;
	return;
}