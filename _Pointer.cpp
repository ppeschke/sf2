#include "_Pointer.h"
#include "Global.h"

_Pointer::_Pointer(Sprite* s)
{
	sprite = s;
	clickRecorded = false;
}


_Pointer::~_Pointer(void)
{
}

void _Pointer::KeepInBounds()
{
	Game* thegame = getGame();
	if(thegame->menu.pointer->coords.x < 0)
		thegame->menu.pointer->coords.x = 0;
	if(thegame->menu.pointer->coords.x > thegame->gWindow.Width - 16)
		thegame->menu.pointer->coords.x = (float)thegame->gWindow.Width - 16;
	if(thegame->menu.pointer->coords.y < 0)
		thegame->menu.pointer->coords.y = 0;
	if(thegame->menu.pointer->coords.y > thegame->gWindow.Height - 16)
		thegame->menu.pointer->coords.y = (float)thegame->gWindow.Height - 16;
}