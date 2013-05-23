#include "Global.h"
#include "QuadTree.h"
#include "Ship.h"
#include "Player.h"
#include "StarField.h"
#include <typeinfo>
#include <fstream>
using namespace std;

int randomNumber(int low, int high);
string stateNames[] = {"starting", "running", "choosingShip", "settings", "paused", "over", "quitting", "debugHolding", "debugMoving"};
//void CollisionDetection(Game* thegame);

void Loop(Game* thegame)
{
	ofstream log;
	log.open("frameTimes.csv");
	log << "Process,Render,Frame, run, endStep, collision, reconcile, gameState" << endl;
	DWORD FrameTimer;
	DWORD RenderTimer;
	DWORD ProcessTimer;
	DWORD startingPoint = GetTickCount();
	DWORD deltaTime;
	DWORD reusableTimer = 0;
	DWORD runTimer = 0, endStepTimer = 0, collisionTimer = 0, reconcileTimer = 0;
	//while there's not a quit message from the window and the game hasn't ended
	while(HandleMessages() && thegame->state != over && thegame->state != quitting)
	{
		deltaTime = GetTickCount() - startingPoint;
		startingPoint = GetTickCount();
		thegame->deltaTime = deltaTime;

		FrameTimer = GetTickCount();
		ProcessTimer = GetTickCount();
		Input(thegame);
		//run
		switch(thegame->state)
		{
		case running:
			runTimer = GetTickCount();
			if(thegame->inputdata.p.downState || thegame->inputdata.tab.downState)
				thegame->state = paused;
			/*log << "Run:" << endl;
			log << "Index, mesh, timer" << endl;*/
			for(unsigned int index = 0; index <= thegame->lastIndex; ++index)
			{
				reusableTimer = GetTickCount();
				if(thegame->objects[index] != NULL && !thegame->objects[index]->dead)
				{
					thegame->objects[index]->run(deltaTime / 1000.0f);
					if(thegame->objects[index]->bb)
						thegame->collisionDetection.UpdatePosition(thegame->objects[index]->bb, thegame->objects[index]->bb->myNode);
					/*reusableTimer = GetTickCount() - reusableTimer;
					log << index << "," << (thegame->objects[index]->mesh != NULL?(int)(thegame->objects[index]->mesh->type):0) << "," << reusableTimer << endl;*/
				}
			}
			if(thegame->inputdata.h.downState)	//haxors
			{

			}
			runTimer = GetTickCount() - runTimer;

			//the run function (called above) is for independant objects); objects that follow independant objects need the 
			//independants to get into place before they situate themselves. Thus the endStep function (called below) is called last in the loop,
			//forcing the independants to be settled before the objects that follow them position themselves.
			endStepTimer = GetTickCount();
			/*log << "endStep:" << endl;
			log << "Index, mesh, timer" << endl;*/
			for(unsigned int index = 0; index <= thegame->lastIndex; ++index)
			{
				reusableTimer = GetTickCount();
				if(thegame->objects[index] != NULL && !thegame->objects[index]->dead)
				{
					thegame->objects[index]->endStep(deltaTime / 1000.0f);
					/*reusableTimer = GetTickCount() - reusableTimer;
					log << index << "," << (thegame->objects[index]->mesh != NULL?(int)(thegame->objects[index]->mesh->type):0) << "," << reusableTimer << endl;*/
				}
			}

			if(thegame->sf)
				thegame->sf->endStep(deltaTime / 1000.0f);

			endStepTimer = GetTickCount() - endStepTimer;

			collisionTimer = GetTickCount();
			thegame->collisionDetection.Run();
			collisionTimer = GetTickCount() - collisionTimer;

			reconcileTimer = GetTickCount();
			thegame->reconcileObjects();
			reconcileTimer = GetTickCount() - reconcileTimer;

			if(thegame->inputdata.escape.downState)
				changeGameMode(over);

			if(thegame->state != over && thegame->state != quitting)
				;//thegame->state = debugHolding;
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

			for(unsigned int index = 0; index <= thegame->lastIndex; ++index)
			{
				if(thegame->objects[index] != NULL && !thegame->objects[index]->dead)
				{
					thegame->objects[index]->endStep(deltaTime / 1000.0f);
				}
			}

			if(thegame->sf)
				thegame->sf->endStep(deltaTime / 1000.0f);

			thegame->collisionDetection.Run();

			thegame->reconcileObjects();
			if(!thegame->inputdata.p.downState && !thegame->inputdata.tab.downState)
				thegame->state = running;
			break;
		case debugHolding:
			if(thegame->inputdata.h.downState)
				thegame->state = debugMoving;
			break;
		case debugMoving:
			if(!thegame->inputdata.h.downState)
				thegame->state = running;
		}

		if(thegame->gametype != NULL && thegame->gametype->checkWin())
			changeGameMode(over);
		
		ProcessTimer = GetTickCount() - ProcessTimer;

		RenderTimer = GetTickCount();
		Render(thegame);
		RenderTimer = GetTickCount() - RenderTimer;

		FrameTimer = GetTickCount() - FrameTimer;

		log << ProcessTimer;
		log << "," << RenderTimer;
		log << "," << FrameTimer;
		log << ", " << runTimer << "," << endStepTimer << "," << collisionTimer << "," << reconcileTimer << ",";
		log << stateNames[thegame->state] << endl;
	}
	thegame->state = over;
	log.close();
}