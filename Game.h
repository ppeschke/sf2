#ifndef GAME_H
#define GAME_H

#include <windows.h>
#include <windowsx.h>
#include <vector>
#include <list>
using namespace std;

#include "Base.h"
#include "Mesh.h"
#include "Texture.h"
#include "GameWindow.h"
#include "Camera.h"
#include "MessageQueue.h"
#include "QuadTree.h"
#include "_Menu.h"
#include "InputData.h"
#include "Wave.h"
#include "Arena.h"

enum gameState {starting, running, choosingShip, settings, paused, over, quitting, debugHolding, debugMoving};
						//over is different from quitting, over is when the gameplay has finished and another game may now start (transition), or you may quit
enum gameType {ctf, ffa};

class CursorTarget;
class Ship;
class GameType;

class Game
{
public:
	Game(void);
	~Game(void);
	void Setup();		//runs after constructor, but also after resource allocation (setting up things that aren't specific to the gametype start, like game menu)
	void Cleanup();
	void insertObject(Base* b);
	void reconcileObjects();
	unsigned int getNextIndex();
	unsigned int getNextPlayer();

	Renderable* getRenderable(renderableType);
	Wave* getSound(soundType);
	void CreateCursor();
	void DestroyCursor();
	void ChangeShip(Ship*);

	unsigned int firstFreeIndex;
	unsigned int lastIndex;
	Player* pc;
	bool invertYaxis;
	GameType* gametype;
	Base* objects[10000];
	list<Base*> addList;
	vector<Renderable*> renderables;
	vector<Wave*> sounds;
	GameWindow gWindow;
	Camera camera;
	MessageQueue messages;
	QuadTree collisionDetection;
	CursorTarget* cursortarget;
	gameState state;
	Player* players[16];	//limit 16, sorry
	_Menu menu;
	_Menu chooseShipMenu;
	InputData inputdata;
	Base* sf;
	Arena* arena;
	Vec2D tempdir, temploc, tempvel;
};

#endif