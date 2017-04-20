#ifndef SERVER
#define SERVER

#include "NetworkAgent.h"
#include "NetworkServer.h"

class Server : public NetworkAgent
{
public:
	Server();
	virtual ~Server();

	NetworkServer server;

	virtual void Input();
	virtual void Output();
	virtual void Setup(HINSTANCE hInstance);
	virtual void Teardown();
};

#endif