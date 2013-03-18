#include "_Button.h"

_Button::_Button(string t, Vec2D c, Vec2D s, void(*a)(int), int p, Sprite* o)
{
	text = t;
	coords = c;
	size = s;
	Action = a;
	param = p;
	outline = o;
}


_Button::~_Button(void)
{
}

bool _Button::Check(Vec2D click)
{
	//bounds check for a click (was it inside the button or not
	if(click.x > coords.x && click.x < coords.x + size.x && click.y > coords.y && click.y < coords.y + size.y)
		return true;
	return false;
}