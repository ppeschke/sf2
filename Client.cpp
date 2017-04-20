#include "Client.h"

#include "GameType.h"

int randomNumber(int low, int high);
bool HandleMessages();
void changeGameMode(int gm);
void Input(Game* thegame);
void Render(Game* thegame);
void LoadGraphics(Game* thegame);
void LoadSounds(Game* thegame);
void InitDirectInput(HINSTANCE, Game*);
void CloseDirectInput();
void CloseDirect3D();
void CloseXAudio();

Client::Client() : NetworkAgent(), client("127.0.0.1")
{
	client.Knock();
}

Client::~Client()
{
}

void Client::Setup(HINSTANCE hInstance)
{
	LoadGraphics(&theGame);
	LoadSounds(&theGame);
	InitDirectInput(hInstance, &theGame);
	//SplashScreen(getGame());
	theGame.Setup();

	//LoopSound(NULL);
}

void Client::Teardown()
{
	CloseDirect3D();
	CloseDirectInput();
	CloseXAudio();
}

void Client::Input()
{
	::Input(&theGame);
}

void Client::Output()
{
	::Render(&theGame);
}