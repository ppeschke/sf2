#ifndef SPRITE_H
#define SPRITE_H

#include "Global.h"

#include "renderable.h"

class Sprite : public Renderable
{
public:
	Sprite(void);
	virtual ~Sprite(void);

	virtual void Render(Vec2D*, Vec2D*);

	LPDIRECT3DTEXTURE9 sprite;    // the pointer to the sprite
};

#endif