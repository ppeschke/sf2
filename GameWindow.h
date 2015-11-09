#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#define WIN32_LEAN_AND_MEAN
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