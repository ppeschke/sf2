#include "Player.h"

Player::Player(unsigned int playerNum, unsigned int t, Ship* s)
{
	ship = s;
	team = t;
	name = "Default Player";
	number = playerNum;
	points = 0;
	for(int i = 0; i < 6; ++i)
		shipLevels[i] = 1;
	lastShip = 0;
}

Player::~Player(void)
{
}
