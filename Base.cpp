#include "Base.h"
#include "Mesh.h"
#include "Global.h"
#include "Thruster.h"
#include "Launcher.h"
#include "GameType.h"
#include "Color.h"
#include "Player.h"
#include "Flag.h"
#include "SpawnPoint.h"
#include "Team.h"

Base::Base(unsigned int _i, Player* o, Vec2D location, Vec2D direction, bool collides, bool draw)
{
	index = _i;
	owner = o;
	loc = location;
	dir = direction;
	collidable = collides;
	dead = false;
	drawMe = draw;
	bb = NULL;
	mesh = NULL;
}

Base::~Base(void)
{
	if(mesh)
		delete mesh;
	if(bb)
		delete bb;
}

void Base::Kill()
{
	dead = true;
	if(mesh != NULL)
	{
		if(typeid(*mesh) == typeid(Mesh))
		{
			Mesh* m = (Mesh*)(this->mesh);
			for(unsigned int i = 0; i < m->mountNum; ++i)
			{
				m->actualMounts[i]->Kill();
				m->actualMounts[i] = NULL;
			}
		}
	}
}

Mesh* Base::CopyMesh(renderableType mT, const Color& c)
{
	if(mT > sse || mT == none)
		return NULL;

	Renderable* temp = getGame()->getRenderable(mT);
	if(temp == NULL)
		return NULL;

	if(typeid(*temp) == typeid(Mesh))
	{
		//copy temp to m
		Mesh* m = new Mesh( *((Mesh*)(temp)) );

		//mesh->polys[i].length denotes the number of vertices in the polygon
		for(unsigned int i = 0; i < m->polyNum; ++i)
		{
			for(unsigned int index = 0; index < m->polys[i].length; ++index)
				m->polys[i].vertices[index].COLOR = D3DCOLOR_XRGB((int)(c.red * 255), (int)(c.green * 255), (int)(c.blue * 255));	//set color
		}
		//load to video memory
		LoadToVRAM(m);

		if(m->mountNum > 0)
		{
			Game* g = getGame();
			m->actualMounts = new Base*[m->mountNum];
			for(unsigned int i = 0; i < m->mountNum; ++i)
			{
				if(m->mountNames[i] == "thruster")
				{
					Base* t = new Thruster(getGame()->getNextIndex(), owner, none, m->mountLocs[i], dir, this, false, true);	//none is used because the default mesh should point to this for the Render function to be called
																												//this->texture is used instead to store the actual drawing
					g->insertObject(t);
					m->actualMounts[i] = t;
				}
				else
					m->actualMounts[i] = NULL;
				//other mounts are handled in the ship class (why?)(so I can control ROF's)
			}
		}
		else
			m->actualMounts = NULL;

		//back to renderable pointer
		return m;
	}
	else
		return NULL;
}