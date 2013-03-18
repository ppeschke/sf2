#ifndef COLOR_H
#define COLOR_H

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
	float red, green, blue;
};

#endif