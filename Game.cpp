#include "GameType.h"
#include <typeinfo>
#include "CursorTarget.h"
#include "Global.h"
#include "Player.h"
#include "Ship.h"
#include "Launcher.h"

Game::Game(void) : collisionDetection(1024, 3)
{
	state = starting;
	messages.x = 5;
	messages.y = 5;
	collisionDetection.thegame = this;
	invertYaxis = false;
	cursortarget = NULL;

	for(int i = 0; i < 16; ++i)
		players[i] = NULL;

	firstFreeIndex = 0;
	lastIndex = 0;

	for(unsigned int i = 0; i < 10000; ++i)
		objects[i] = NULL;
}

Game::~Game(void)
{
	Cleanup();
	if(gametype)
		delete gametype;
	for(int i = 0; i < 16; ++i)
		if(players[i])
			delete players[i];
	if(sf)
		delete sf;

	if(arena)
		delete arena;
}

void Game::Setup()
{
	/*_Button x;
	x.coords = Vec2D(gWindow.Width/2 - 125, 100.0f);
	x.text = "Join Server";
	x.Action = NULL;
	x.param = 0;
	x.outline = (Sprite*)(getRenderable(buttonOutline));
	menu.buttons.insert(menu.buttons.end(), x);*/

	menu.buttons.insert(menu.buttons.end(), _Button("Capture The Flag", Vec2D((float)(gWindow.Width/2 - 125), 100.0f), Vec2D(250.0f, 50.0f), &GoToSettings, (int)ctf, (Sprite*)(getRenderable(buttonOutline))));
	menu.buttons.insert(menu.buttons.end(), _Button("Exit", Vec2D((float)(gWindow.Width/2 - 125), (float)(gWindow.Height - 100)), Vec2D(250.0f, 50.0f), &changeGameMode, (int)quitting, (Sprite*)getRenderable(buttonOutline)));
	menu.headings.insert(menu.headings.end(), _Heading("Choose Game Type", Vec2D(gWindow.Width/2 - (16/2.0f*16), 50)));
	menu.pointer = new _Pointer((Sprite*)getRenderable(mousePointer));

	chooseShipMenu.headings.insert(chooseShipMenu.headings.end(), _Heading("Choose Your Ship", Vec2D(gWindow.Width/2 - (16/2.0f*16), 50)));
	chooseShipMenu.buttons.insert(chooseShipMenu.buttons.end(), _Button("Scout", Vec2D((float)(gWindow.Width/2 - 125), 100.0f), Vec2D(250.0f, 50.0f), &MakeNewShip, (int)scout, (Sprite*)(getRenderable(buttonOutline))));
	chooseShipMenu.buttons.insert(chooseShipMenu.buttons.end(), _Button("Recon", Vec2D((float)(gWindow.Width/2 - 125), 200.0f), Vec2D(250.0f, 50.0f), &MakeNewShip, (int)recon, (Sprite*)(getRenderable(buttonOutline))));
	chooseShipMenu.buttons.insert(chooseShipMenu.buttons.end(), _Button("Heavy", Vec2D((float)(gWindow.Width/2 - 125), 300.0f), Vec2D(250.0f, 50.0f), &MakeNewShip, (int)heavy, (Sprite*)(getRenderable(buttonOutline))));
	chooseShipMenu.buttons.insert(chooseShipMenu.buttons.end(), _Button("Sniper", Vec2D((float)(gWindow.Width/2 - 125), 400.0f), Vec2D(250.0f, 50.0f), &MakeNewShip, (int)sniper, (Sprite*)(getRenderable(buttonOutline))));
	chooseShipMenu.buttons.insert(chooseShipMenu.buttons.end(), _Button("E-War", Vec2D((float)(gWindow.Width/2 - 125), 500.0f), Vec2D(250.0f, 50.0f), &MakeNewShip, (int)ewar, (Sprite*)(getRenderable(buttonOutline))));
	chooseShipMenu.buttons.insert(chooseShipMenu.buttons.end(), _Button("Logistics", Vec2D((float)(gWindow.Width/2 - 125), 600.0f), Vec2D(250.0f, 50.0f), &MakeNewShip, (int)logistics, (Sprite*)(getRenderable(buttonOutline))));
	chooseShipMenu.pointer = new _Pointer((Sprite*)getRenderable(mousePointer));
}

void Game::Cleanup()
{
	//just take care of objects and renderables in here,
	//everything else is taken care of in the game destructor
	for(vector<Renderable*>::iterator index = renderables.begin(); index != renderables.end(); ++index)
	{
		if((*index) != NULL)
		{
			delete (*index);
			(*index) = NULL;
		}
	}
	for(unsigned int index = 0; index <= lastIndex; ++index)
	{
		if(objects[index] != NULL)
		{
			delete objects[index];
			objects[index] = NULL;
		}
	}
	for(vector<Wave*>::iterator index = sounds.begin(); index != sounds.end(); ++index)
	{
		if((*index) != NULL)
		{
			delete (*index);
			(*index) = NULL;
		}
	}
}

void Game::ChangeShip(Ship* s)
{
	camera.targetZ = -s->sensorStrength;
	pc->ship = s;
	if(pc->ship->weapon != NULL && (((Launcher*)pc->ship->weapon)->shootWhat == shootMissile || ((Launcher*)pc->ship->weapon)->shootWhat == shootDrone))
		CreateCursor();
	else
		DestroyCursor();
}

Renderable* Game::getRenderable(renderableType m)
{
	Renderable* temp = NULL;
	//returns the first one it finds
	for(vector<Renderable*>::iterator index = renderables.begin(); index != renderables.end(); ++index)
	{
		if((*index)->type == m)
		{
			temp = (*index);
			break;
		}
	}
	return temp;
}

void Game::reconcileObjects()
{
	//remove objects that are dead
	for(unsigned int index = 0; index <= lastIndex; ++index)
	{
		if(objects[index] && objects[index]->dead)
		{
			if(objects[index]->collidable)
				collisionDetection.Remove(objects[index]->bb);	//remove the boundingBox
			delete objects[index];								//destroy the object
			objects[index] = NULL;							//remove the trash

			if(index < firstFreeIndex)
				firstFreeIndex = index;
			if(index == lastIndex)	//deleted last item
				while(lastIndex >= 0 && objects[lastIndex] == NULL)		//until we come to the beginning or find an object
					--lastIndex;	//move back lastIndex
		}
	}
	//add new objects generated in this frame
	for(list<Base*>::iterator index = addList.begin(); index != addList.end(); ++index)
	{
		objects[firstFreeIndex] = (*index);	//server side
		objects[firstFreeIndex]->index = firstFreeIndex;	//just making sure

		if(firstFreeIndex == lastIndex + 1)	//next available spot is at the end of what we've stored (inserted at end
			++lastIndex;
		
		while(firstFreeIndex <= 9999 && objects[firstFreeIndex] != NULL)
			++firstFreeIndex;
		if(firstFreeIndex == 9999)
			exit(EXIT_FAILURE);	//crash hard

		if((*index)->collidable)
			collisionDetection.Insert((*index)->bb);
	}
	addList.clear();
}

void Game::insertObject(Base* b)
{
	//this function adds the objects to the addList so they can be inserted between frames
	addList.insert(addList.end(), b);
}

void Game::CreateCursor()
{
	if(!this->cursortarget)
	{
		this->cursortarget = new CursorTarget(getGame()->getNextIndex(), this->pc, target, Vec2D(), Vec2D(0.0f, 1.0f), true, true);
		this->insertObject(this->cursortarget);
	}
}

void Game::DestroyCursor()
{
	if(this->cursortarget)
		this->cursortarget->Kill();
	cursortarget = NULL;
}

unsigned int Game::getNextIndex()
{
	return firstFreeIndex;
}

unsigned int Game::getNextPlayer()
{
	for(unsigned int i = 0; i < 16; ++i)
		if(players[i] == NULL)
			return i;
	return -1;
}

Wave* Game::getSound(soundType s)
{
	for(vector<Wave*>::iterator index = sounds.begin(); index != sounds.end(); ++index)
	{
		if(s == (*index)->sT)
			return (*index);
	}
	return NULL;
}