#include "Polygon.h"
using namespace sf2;

#define NULL 0

Polygon::Polygon(void)
{
	length = 0;
	vertices = NULL;
	c = Color(1.0f, 1.0f, 1.0f);
	v_buffer = NULL;
	VRAMloaded = false;
}

Polygon::Polygon(const Polygon& orig)
{
	//haven't made a copy constructor in a long time
	if(this == &orig)
		return;
	this->length = orig.length;
	this->VRAMloaded = orig.VRAMloaded;
	if(length)
	{
		this->vertices = new CUSTOMVERTEX[length];
		for(unsigned int i = 0; i < length; ++i)
		{
			this->vertices[i] = orig.vertices[i];
		}
	}
	else
	{
		this->vertices = NULL;
	}
}

Polygon::~Polygon(void)
{
	if(vertices)
	{
		delete [] vertices;
	}
	if(VRAMloaded)
		v_buffer->Release();
}

sf2::Polygon& sf2::Polygon::operator=(const Polygon& orig)
{
	//haven't made a copy constructor in a long time
	if(this == &orig)
		return (*this);
	this->length = orig.length;
	this->VRAMloaded = orig.VRAMloaded;
	if(this->vertices)
	{
		delete [] this->vertices;
	}
	if(length)
	{
		this->vertices = new CUSTOMVERTEX[length];
		for(unsigned int i = 0; i < length; ++i)
		{
			this->vertices[i] = orig.vertices[i];
		}
	}
	else
	{
		this->vertices = NULL;
	}
	return (*this);
}