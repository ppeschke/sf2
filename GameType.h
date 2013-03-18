#ifndef GAMETYPE_H
#define GAMETYPE_H

#include <string>
using namespace std;

#include "Renderable.h"
#include "_Menu.h"

class Game;
class Team;
class Base;
class Ship;

class GameType
{
public:
	GameType(void);
	virtual ~GameType(void);

	virtual void OnStart(Game* thegame);
	virtual void onCollision(Base* a, Base* b);
	virtual Ship* SpawnShip(renderableType mT, unsigned int playerNum, Vec2D dir = Vec2D(0.0f, 1.0f), Vec2D vel = Vec2D());
	virtual Ship* RespawnShip(Vec2D loc, Vec2D dir, Vec2D vel, renderableType mT, unsigned int playerNum);
	virtual void HandleDeath(Ship* b);
	virtual bool checkWin();

	void changeObjective(int points);
	void changeTeamPlayers(int members);

	unsigned int pointsToWin;
	unsigned int playersPerTeam;

	string objectives;
	string name;
	_Menu settingsMenu;
	_Menu pauseMenu;
	
	Team* teams;
};
#endif