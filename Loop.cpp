#include "Global.h"
#include "QuadTree.h"
#include "Ship.h"
#include "Player.h"
#include "StarField.h"
#include <typeinfo>
#include <fstream>
using namespace std;

int randomNumber(int low, int high);
//void CollisionDetection(Game* thegame);

void Loop(Game* thegame)
{
	ofstream log;
	log.open("frameTimes.csv");
	log << "Process,Render,Frame" << endl;
	DWORD FrameTimer;
	DWORD RenderTimer;
	DWORD ProcessTimer;
	DWORD startingPoint = GetTickCount();
	DWORD deltaTime;
	//while there's not a quit message from the window and the game hasn't ended
	while(HandleMessages() && thegame->state != over && thegame->state != quitting)
	{
		deltaTime = GetTickCount() - startingPoint;
		startingPoint = GetTickCount();

		FrameTimer = GetTickCount();
		ProcessTimer = GetTickCount();
		Input(thegame);
		//run
		switch(thegame->state)
		{
		case running:
			if(thegame->inputdata.p.downState || thegame->inputdata.tab.downState)
				thegame->state = paused;
			for(unsigned int index = 0; index <= thegame->lastIndex; ++index)
			{
				if(thegame->objects[index] != NULL && !thegame->objects[index]->dead)
				{
					thegame->objects[index]->run(deltaTime / 1000.0f);
					if(thegame->objects[index]->bb)
						thegame->collisionDetection.UpdatePosition(thegame->objects[index]->bb, thegame->objects[index]->bb->myNode);
				}
			}
			if(thegame->sf)
				thegame->sf->run(deltaTime / 1000.0f);

			thegame->collisionDetection.Run();
			if(thegame->inputdata.h.downState)	//haxors
			{
			}

			thegame->reconcileObjects();
			ProcessTimer = GetTickCount() - ProcessTimer;
			if(thegame->inputdata.escape.downState)
				changeGameMode(over);
			break;
			/***********************************END RUNNING**************************************/
		case choosingShip:
			thegame->chooseShipMenu.run();
			if(thegame->inputdata.escape.downState)
				changeGameMode(over);
			break;
			/*********************************END CHOOSING SHIP**********************************/
		case starting:
			thegame->menu.run();
			if(thegame->inputdata.escape.downState)
				changeGameMode(over);
			break;
			/**********************************END STARTING**************************************/
		case settings:
			getGameType()->settingsMenu.run();
			if(thegame->inputdata.escape.downState)
				changeGameMode(over);
			break;
			/*********************************END SETTINGS*************************************/
		case paused:
			for(unsigned int index = 0; index <= thegame->lastIndex; ++index)
			{
				if(thegame->objects[index] != NULL && !thegame->objects[index]->dead)
				{
					thegame->objects[index]->run(deltaTime / 1000.0f);
					if(thegame->objects[index]->bb)
						thegame->collisionDetection.UpdatePosition(thegame->objects[index]->bb, thegame->objects[index]->bb->myNode);
				}
			}

			if(thegame->sf)
				thegame->sf->run(deltaTime / 1000.0f);

			thegame->collisionDetection.Run();

			thegame->reconcileObjects();
			ProcessTimer = GetTickCount() - ProcessTimer;
			if(!thegame->inputdata.p.downState && !thegame->inputdata.tab.downState)
				thegame->state = running;
			break;
		}

		if(thegame->gametype != NULL && thegame->gametype->checkWin())
			changeGameMode(over);

		RenderTimer = GetTickCount();
		Render(thegame);
		RenderTimer = GetTickCount() - RenderTimer;

		FrameTimer = GetTickCount() - FrameTimer;

		log << ProcessTimer;
		log << "," << RenderTimer;
		log << "," << FrameTimer << endl;
	}
	thegame->state = over;
	log.close();
}