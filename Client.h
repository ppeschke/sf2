#ifndef CLIENT
#define CLIENT

#include "NetworkAgent.h"
#include "NetworkClient.h"

class Client : public NetworkAgent
{
public:
	Client();
	virtual ~Client();

	NetworkClient client;

	virtual void Input();
	virtual void Output();
	virtual void Setup(HINSTANCE hInstance);
	virtual void Teardown();
};

#endif