#ifndef PLAYER_H
#define PLAYER_H

#include <string>
using namespace std;

class Ship;

class Player
{
public:
	Player(unsigned int playerNum, unsigned int t, Ship*);
	virtual ~Player(void);

	Ship* ship;
	unsigned int team;
	string name;
	unsigned int points;

	unsigned int number;
	unsigned int shipLevels[6];
	unsigned int lastShip;
};

#endif