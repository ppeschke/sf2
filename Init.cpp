#include "Global.h"
#include "Mesh.h"

/*
TODO: include gametypes (the .h files) here and impliment a choosing mechanism in the menu function
*/

void Menu(Game* thegame)
{
	GameType* CTF = new GameType;
	thegame->gametype = CTF;
	thegame->gametype->OnStart(thegame);
}

void SplashScreen(Game* thegame)
{	
	DWORD Timer = GetTickCount();
	DWORD Count = 0;
	int c = 0;
	int in = 0;
	//fade in and squish the letters together
	while(HandleMessages() && Count < 3000)
	{
		if(c < 255)
			++c;
		if(in < 10 && c % 5 == 0)
			++in;
		StartRender();
		drawText("s", thegame->gWindow.Width/2 - 20 - 5 + in, thegame->gWindow.Height/2 - 8, 0, 255, 0);
		drawText("f", thegame->gWindow.Width/2 - 5,           thegame->gWindow.Height/2 - 8, 0, 255, 0);
		drawText("2", thegame->gWindow.Width/2 + 20 - 5 - in, thegame->gWindow.Height/2 - 8, 0, 255, 0);
		Draw2DLine(-70, 30, 70, 30, D3DCOLOR_XRGB(0, c, 0));
		Draw2DLine(70, 30, 70, -30, D3DCOLOR_XRGB(0, c, 0));
		Draw2DLine(-70, -31, 70, -31, D3DCOLOR_XRGB(0, c, 0));
		Draw2DLine(-70, 30, -70, -30, D3DCOLOR_XRGB(0, c, 0));
		EndRender();
		Count = GetTickCount() - Timer;
	}
	//fade out
	while(HandleMessages() && c >= 0)
	{
		StartRender();
		drawText("s", thegame->gWindow.Width/2 - 20 - 5 + in, thegame->gWindow.Height/2 - 8, 0, c, 0);
		drawText("f", thegame->gWindow.Width/2 - 5,           thegame->gWindow.Height/2 - 8, 0, c, 0);
		drawText("2", thegame->gWindow.Width/2 + 20 - 5 - in, thegame->gWindow.Height/2 - 8, 0, c, 0);
		Draw2DLine(-70, 30, 70, 30, D3DCOLOR_XRGB(0, c, 0));
		Draw2DLine(70, 30, 70, -30, D3DCOLOR_XRGB(0, c, 0));
		Draw2DLine(-70, -31, 70, -31, D3DCOLOR_XRGB(0, c, 0));
		Draw2DLine(-70, 30, -70, -30, D3DCOLOR_XRGB(0, c, 0));
		EndRender();
		--c;
	}
}