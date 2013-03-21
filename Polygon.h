#ifndef POLYGON_H
#define POLYGON_H

//needed for FLOAT and DWORD
#include <windows.h>
#include "Color.h"

namespace sf2	//I just know that this is going to cause some problems
{
	struct CUSTOMVERTEX
	{
		FLOAT X, Y, Z;
		DWORD COLOR;
	};

	class Polygon
	{
	public:
		Polygon(void);
		Polygon(const Polygon& orig);
		~Polygon(void);
		Polygon& operator=(const Polygon& orig);

		CUSTOMVERTEX* vertices;
		unsigned int length;
		LPDIRECT3DVERTEXBUFFER9 v_buffer;
		Color c;
		bool VRAMloaded;
	};
}

#endif
