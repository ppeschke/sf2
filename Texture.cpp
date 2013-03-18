#include "Texture.h"

#include "Global.h"


Texture::Texture()
{
}


Texture::~Texture(void)
{
	t_vertex->Release();
	texture->Release();
}

void Texture::Render(Vec2D* loc, Vec2D* dir)
{
}