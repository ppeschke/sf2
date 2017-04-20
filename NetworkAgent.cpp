#include "NetworkAgent.h"
#include "GameType.h"

int randomNumber(int low, int high);
bool HandleMessages();
void changeGameMode(int gm);
void Input(Game* thegame);
void Render(Game* thegame);
void LoadGraphics(Game* thegame);
void LoadSounds(Game* thegame);
void InitDirectInput(HINSTANCE, Game*);
void CloseDirectInput();
void CloseDirect3D();
void CloseXAudio();

NetworkAgent::NetworkAgent() : theGame()
{
}

NetworkAgent::~NetworkAgent()
{
}

void NetworkAgent::Loop()
{
	Game* thegame = &this->theGame;	//saving myself from rewriting all the code in this function (it originally came from Loop.cpp)
	DWORD startingPoint = GetTickCount();
	DWORD deltaTime;
	//while there's not a quit message from the window and the game hasn't ended
	while(HandleMessages() && thegame->state != over && thegame->state != quitting)
	{
		thegame->debug.StartFrame();

		deltaTime = GetTickCount() - startingPoint;
		startingPoint = GetTickCount();
		thegame->deltaTime = deltaTime;

		this->Input();

		//run
		switch(thegame->state)
		{
		case running:
			thegame->debug.EnterSubProcess("Process", 10);
			if(thegame->inputdata.p.downState || thegame->inputdata.tab.downState)
				thegame->state = paused;
			thegame->debug.EnterSubProcess("Run", 10);
			for(unsigned int index = 0; index <= thegame->lastIndex; ++index)
			{
				if(thegame->objects[index] != NULL && !thegame->objects[index]->dead)
				{
					thegame->objects[index]->run(deltaTime / 1000.0f);
					if(thegame->objects[index]->bb)
						thegame->collisionDetection.UpdatePosition(thegame->objects[index]->bb, thegame->objects[index]->bb->myNode);
				}
			}
			thegame->debug.ExitSubProcess();
			if(thegame->inputdata.h.downState)	//haxors
			{

			}

			//the run function (called above) is for independant objects); objects that follow independant objects need the 
			//independants to get into place before they situate themselves. Thus the endStep function (called below) is called last in the loop,
			//forcing the independants to be settled before the objects that follow them position themselves.
			thegame->debug.EnterSubProcess("EndStep", 10);
			for(unsigned int index = 0; index <= thegame->lastIndex; ++index)
			{
				if(thegame->objects[index] != NULL && !thegame->objects[index]->dead)
				{
					thegame->objects[index]->endStep(deltaTime / 1000.0f);
				}
			}

			if(thegame->sf)
				thegame->sf->endStep(deltaTime / 1000.0f);

			thegame->debug.ExitSubProcess();

			thegame->debug.EnterSubProcess("Collisions", 10);
			thegame->collisionDetection.Run();
			thegame->debug.ExitSubProcess();

			thegame->debug.EnterSubProcess("Reconcile", 10);
			thegame->reconcileObjects();
			thegame->debug.ExitSubProcess();

			if(thegame->inputdata.escape.downState)
				changeGameMode(over);

			//if(thegame->state != over && thegame->state != quitting)
			//thegame->state = debugHolding;

			thegame->debug.ExitSubProcess();
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
			thegame->gametype->settingsMenu.run();
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

		this->Output();

		thegame->debug.EndFrame();
	}
	thegame->state = over;
	//log.close();
}

void NetworkAgent::Setup(HINSTANCE hInstance)
{
}

void NetworkAgent::Teardown()
{
}
