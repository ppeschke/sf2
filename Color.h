#ifndef COLOR_H
#define COLOR_H

#include <d3dx9.h>
#include <d3d9.h>

struct Color
{
	Color()
	{
		red = 1.0f;
		green = 1.0f;
		blue = 1.0f;
	}
	Color(float r, float g, float b)
	{
		red = r;
		green = g;
		blue = b;
	}
	DWORD returnD3DCOLOR() const
	{
		return D3DCOLOR_XRGB((int)(red * 255), (int)(green * 255), (int)(blue * 255));
	}

	float red, green, blue;
};

#endif