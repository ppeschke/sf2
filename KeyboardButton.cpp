#include "KeyboardButton.h"


KeyboardButton::KeyboardButton(void)
{
	hitStart = downState = hitEvent = false;
	coords = Vec2D();
}


KeyboardButton::~KeyboardButton(void)
{
}

void KeyboardButton::ResetEvent()
{
	hitEvent = false;
	hitStart = false;
}