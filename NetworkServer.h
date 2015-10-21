#ifndef NETWORK_SERVER
#define NETWORK_SERVER

#include "NetworkAgent.h"

class NetworkServer : public NetworkAgent
{
public:
	NetworkServer();
	virtual ~NetworkServer();

	virtual void Loop();
	virtual void Input();
	virtual void Output();
	virtual void Setup(HINSTANCE hInstance);
	virtual void Teardown();
};

#endif