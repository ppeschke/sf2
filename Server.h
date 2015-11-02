#ifndef SERVER
#define SERVER

#include "NetworkAgent.h"

class Server : public NetworkAgent
{
public:
	Server();
	virtual ~Server();

	virtual void Loop();
	virtual void Input();
	virtual void Output();
	virtual void Setup(HINSTANCE hInstance);
	virtual void Teardown();
};

#endif