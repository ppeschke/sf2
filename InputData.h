#ifndef INPUTDATA_H
#define	INPUTDATA_H

#include "MouseInput.h"

class InputData
{
public:
	InputData(void);
	~InputData(void);

	KeyboardButton w;
	KeyboardButton a;
	KeyboardButton s;
	KeyboardButton d;
	KeyboardButton left;
	KeyboardButton right;
	KeyboardButton up;
	KeyboardButton down;
	KeyboardButton r;
	KeyboardButton space;
	KeyboardButton h;	//haxors
	KeyboardButton y;	//break lock
	KeyboardButton tab;	//pause
	KeyboardButton escape;
	KeyboardButton p;	//pause
	MouseInput mouse;

};

#endif