#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <Windows.h>	//for HWND

struct GameWindow
{
	HWND hWnd;
	int Width, Height;
	bool Windowed;
	int fov;

	GameWindow();
};

#endif