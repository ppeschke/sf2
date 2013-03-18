#include "_Menu.h"
#include "Global.h"

_Menu::_Menu(void)
{
	backdrop = NULL;
	pointer = NULL;
}


_Menu::~_Menu(void)
{
	if(backdrop)
		delete backdrop;
	if(pointer)
		delete pointer;
}

void _Menu::Render()
{
	StartSpriteRender();
	//backdrop
	if(backdrop)
		DrawSprite(backdrop->sprite, (int)backdrop->coords.x, (int)backdrop->coords.y);

	//button outlines
	for(list<_Button>::iterator index = buttons.begin(); index != buttons.end(); ++index)
		DrawSprite((*index).outline, (int)(*index).coords.x, (int)(*index).coords.y);

	//pictures
	for(list<_Picture>::iterator index = pictures.begin(); index != pictures.end(); ++index)
		DrawSprite((*index).sprite, (int)(*index).coords.x, (int)(*index).coords.y);

	EndSpriteRender();
	
	//button text
	for(list<_Button>::iterator index = buttons.begin(); index != buttons.end(); ++index)
		drawText((*index).text.c_str(), (int)(*index).coords.x + 15, (int)(*index).coords.y + 18, 255, 255, 255);

	//headings
	for(list<_Heading>::iterator index = headings.begin(); index != headings.end(); ++index)
		drawBigText((*index).text.c_str(), (int)(*index).coords.x, (int)(*index).coords.y, 255, 255, 0);

	//numbers (scores)
	for(list<_DynamicNumber>::iterator index = numbers.begin(); index != numbers.end(); ++index)
	{
		if((*index).big)
			drawBigText(itos(*(*index).num).c_str(), (int)(*index).coords.x, (int)(*index).coords.y, 255, 255, 0);
		else
			drawText(itos(*(*index).num).c_str(), (int)(*index).coords.x, (int)(*index).coords.y, 255, 255, 0);
	}

	//labels
	for(list<_Label>::iterator index = labels.begin(); index != labels.end(); ++index)
		drawText((*index).text.c_str(), (int)(*index).coords.x, (int)(*index).coords.y, 255, 255, 0);

	StartSpriteRender();
	if(pointer)
		DrawSprite(pointer->sprite, (int)pointer->coords.x, (int)pointer->coords.y);
	EndSpriteRender();
}

void _Menu::run()
{
	Game* thegame = getGame();
	pointer->coords += thegame->inputdata.mouse.coords;
	pointer->KeepInBounds();
	if(thegame->inputdata.mouse.left.downState && !pointer->clickRecorded)
	{
		pointer->clickCoords = pointer->coords;
		pointer->clickRecorded = true;
	}
	if(thegame->inputdata.mouse.left.hitEvent)
	{
		for(list<_Button>::iterator index = buttons.begin(); index != buttons.end(); ++index)
		{
			if((*index).Check(pointer->clickCoords) && (*index).Check(pointer->coords))	//if pointer was in the button on the start of the click and also now,
				(*index).Action((*index).param);						//call the button's Action()
		}
		thegame->inputdata.mouse.left.ResetEvent();
		pointer->clickRecorded = false;
	}
}