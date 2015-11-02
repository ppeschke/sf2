#ifndef CLIENT
#define CLIENT

#include "NetworkAgent.h"

class Client : public NetworkAgent
{
public:
	Client();
	virtual ~Client();

	virtual void Loop();
	virtual void Input();
	virtual void Output();
	virtual void Setup(HINSTANCE hInstance);
	virtual void Teardown();
};

#endif