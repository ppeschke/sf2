#ifndef NETWORK_AGENT
#define NETWORK_AGENT

#include "Game.h"

class NetworkAgent
{
public:
	NetworkAgent();
	virtual ~NetworkAgent();

	virtual void Loop() = 0;
	virtual void Input() = 0;
	virtual void Output() = 0;
	virtual void Setup(HINSTANCE hInstance);
	virtual void Teardown();
	Game theGame;
};

#endif