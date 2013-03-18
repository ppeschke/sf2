#ifndef TEAM_H
#define TEAM_H

#include <list>
#include <string>
using namespace std;

#include "Color.h"
class SpawnPoint;
class Flag;
class Ship;

class Team
{
public:
	Team(void);
	~Team(void);

	list<unsigned int> players;
	void addPlayer(unsigned int playerNumber);
	
	Color color;
	unsigned int number;
	unsigned int points;
	unsigned int playersAllowed;
	string name;

	SpawnPoint* spawnpoint;
	Flag* flag;
	Ship* carryingFlag;
};

#endif