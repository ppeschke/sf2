#ifndef MESSAGE_H
#define MESSAGE_H

#include "Color.h"

struct Message
{
	char m[256];
	Color c;
	
	Message(const char* mess, Color col)
	{
		strcpy_s(m, 256, mess);
		c = col;
	}
};

#endif
