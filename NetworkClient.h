#ifndef NETWORK_CLIENT
#define NETWORK_CLIENT

#include "NetworkAgent.h"

class NetworkClient : public NetworkAgent
{
public:
	NetworkClient();
	virtual ~NetworkClient();

	virtual void Loop();
	virtual void Input();
	virtual void Output();
	virtual void Setup(HINSTANCE hInstance);
	virtual void Teardown();
};

#endif