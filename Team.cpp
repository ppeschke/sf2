#include "Team.h"

Team::Team()
{
	spawnpoint = NULL;
	flag = NULL;
	carryingFlag = NULL;
	points = 0;
	playersAllowed = 1;
}

Team::~Team(void)
{
}

void Team::addPlayer(unsigned int playerNumber)
{
	if(players.size() < playersAllowed)
		players.insert(players.end(), playerNumber);
}