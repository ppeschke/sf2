#include "Global.h"
#include "Ship.h"
#include "CursorTarget.h"
#include "Vec2D.h"
#include "Player.h"
/*********************************************************
	TODO:
	- add modifier checking (shift/alt/ctrl etc) ?
	- also reconfigure it to work with network/server stuff, and maybe a "state" based system
	  as we discussed
	- ????
	- profit
*********************************************************/

#define keymacro(code, keydata); id->keydata.downState = code != 0;if(id->keydata.downState && !id->keydata.hitStart){if(!id->keydata.hitStart)id->keydata.hitStart = true;}if(!id->keydata.downState && id->keydata.hitStart)id->keydata.hitEvent = true;
/*
this is what the keymacro does

id->keydata.downState = code;
if(id->keydata.downState && !id->keydata.hitStart)
{
	if(!id->keydata.hitStart)
		id->keydata.hitStart = true;
}
if(!id->keydata.downState && id->keydata.hitStart)
	id->keydata.hitEvent = true;
*/

void Input(Game* thegame)
{
	InputData* id = &thegame->inputdata;
	static BYTE Keys[256];
	static DIMOUSESTATE Mouse;
	GetKeys(&Keys[0]);
	GetMouse(&Mouse);
	switch(thegame->state)
	{
	case debugHolding:
	case debugMoving:
	case running:
		keymacro(Keys[DIK_P], p);
		keymacro(Keys[DIK_TAB], tab);

		keymacro(Keys[DIK_Y], y);
		keymacro(Keys[DIK_LEFT], left);
		keymacro(Keys[DIK_RIGHT], right);
		keymacro(Keys[DIK_UP], up);
		keymacro(Keys[DIK_DOWN], down);
		keymacro(Keys[DIK_A], a);
		keymacro(Keys[DIK_D], d);
		keymacro(Keys[DIK_W], w);
		keymacro(Keys[DIK_S], s);
		keymacro(Keys[DIK_SPACE], space);
		keymacro(Mouse.rgbButtons[0], mouse.left);
		keymacro(Keys[DIK_R], r);
		keymacro(Keys[DIK_H], h);
		keymacro(Keys[DIK_ESCAPE], escape);

		id->mouse.coords = Vec2D((float)Mouse.lX, (float)(thegame->invertYaxis?Mouse.lY:-Mouse.lY));

		break;
	case paused:
		keymacro(Keys[DIK_P], p);
		keymacro(Keys[DIK_TAB], tab);
		break;
	case choosingShip:
		keymacro(Keys[DIK_ESCAPE], escape);
		keymacro(Mouse.rgbButtons[0], mouse.left);
		id->mouse.coords = Vec2D((float)Mouse.lX, (float)(thegame->invertYaxis?Mouse.lY:-Mouse.lY));
	case starting:
		keymacro(Keys[DIK_ESCAPE], escape);
		keymacro(Mouse.rgbButtons[0], mouse.left);
		id->mouse.coords = Vec2D((float)Mouse.lX, (float)(thegame->invertYaxis? -Mouse.lY:Mouse.lY));
	case settings:
		keymacro(Keys[DIK_ESCAPE], escape);
		keymacro(Mouse.rgbButtons[0], mouse.left);
		id->mouse.coords = Vec2D((float)Mouse.lX, (float)(thegame->invertYaxis? -Mouse.lY:Mouse.lY));
		break;
	}
	return;
}