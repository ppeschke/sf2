#ifndef _MENU_H
#define _MENU_H

#include "_Button.h"
#include "_Label.h"
#include "_Heading.h"
#include "_Pointer.h"
#include "_Picture.h"
#include "_DynamicNumber.h"
#include <list>
using namespace std;

class _Menu
{
public:
	_Menu(void);
	~_Menu(void);

	void Render();
	void run();

	_Pointer* pointer;
	list<_Label> labels;
	list<_Button> buttons;
	list<_Heading> headings;
	list<_Picture> pictures;
	list<_DynamicNumber> numbers;
	_Picture* backdrop;
};

#endif