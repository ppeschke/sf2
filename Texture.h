#ifndef TEXTURE_H
#define TEXTURE_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3dx9.h>
#include <d3d9.h>

#include "renderable.h"

//enum texturetype {orangedot};

class Texture : public Renderable
{
public:
	Texture(void);
	virtual ~Texture(void);

	virtual void Render(Vec2D* loc, Vec2D* dir);

	LPDIRECT3DTEXTURE9 texture;				//pointer to the texture
	LPDIRECT3DVERTEXBUFFER9 t_vertex;		//pointer to the vertices
};

#endif