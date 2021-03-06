#include "Global.h"
#include "Player.h"
#include "Server.h"
#include "Client.h"

#include <time.h>
#include <sstream>
using namespace std;

bool HandleMessages();
void DisplayWindow(GameWindow* gw, HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
string gameStates[] = {"starting", "running", "choosingShip", "settings", "paused", "over", "quitting", "debugHolding", "debugMoving"};

//Game thegame;
NetworkAgent* agent;	//the network agent will contain the game now

// Starting Point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if(strcmp("Server", lpCmdLine) == 0)
		agent = new Server();
	else
		agent = new Client();
	srand((unsigned int)time(NULL));
	//randomFloat(0.0f, 0.0f);randomFloat(0.0f, 0.0f);	//getting the randomness primed
	DisplayWindow(&getGame()->gWindow, hInstance, nCmdShow);	//have to have a window before setting up direct3D
	agent->Setup(hInstance);
	agent->Loop();
	//Loop(&thegame);

	agent->Teardown();

	delete agent;

	return 0;
}

// Create the Window Class and the Window
void DisplayWindow(GameWindow* gw, HINSTANCE hInstance, int nCmdShow)
{
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX)); wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = (WNDPROC)WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = TEXT("WindowClass");

    RegisterClassEx(&wc);

    gw->hWnd = CreateWindowEx(NULL, wc.lpszClassName, TEXT("sf2"),
                              gw->Windowed ? WS_OVERLAPPEDWINDOW : WS_EX_TOPMOST|WS_POPUP,
                              50, 50, gw->Width, gw->Height,
                              NULL, NULL, hInstance, NULL);

    ShowWindow(gw->hWnd, nCmdShow);
    return;
}

// Check for Messages and Handle
bool HandleMessages()
{
    static MSG msg;

    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            return FALSE;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return TRUE;
}

// Process the Messages
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_DESTROY:
            {
                PostQuitMessage(0);
				getGame()->state = quitting;
                return 0;
            } break;
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}

inline Game* getGame()
{
	return &agent->theGame;
}

GameType* getGameType()
{
	return agent->theGame.gametype;
}

int randomNumber(int low, int high)
{
	int randomInteger;
	int range=(high-low)+1;
	randomInteger = low + int(range * rand() / (RAND_MAX + 1.0));
	return randomInteger;
}

float processNumber(string s)
{
	float f = 0.0f;
	unsigned int index = 0;
	bool negative = false;

	if(s[index] == '-')
	{
		negative = true;
		++index;
	}

	while(index < s.size() && isdigit(s[index]))
	{
		f = 10 * f + s[index] - 48;	//add each place onto value until you reach
									//a value that's not a number
		++index;
	}
	if(index < s.size() && s[index] != '.')
		return f;			//if it's not a decimal place, it's not important to this function
	else
		++index;

	float factor = 1.0;		//this is the factor to multiply each place by
								//(it gets smaller by .1 with each new place)
	while(index < s.size() && isdigit(s[index]))
	{
		factor *= 0.1f;		//(it gets smaller by .1 with each new place)
		f = f + (s[index++] - 48) * factor;	//add each new decimal place to value
	}
	return (negative? -f:f);
}

float randomFloat(float low, float high)
{
	float range = high - low;
	float ratio = randomNumber(0, 10000) / 10000.0f;
	return low + (ratio*range);
}

float Distance(const Vec2D& a, const Vec2D& b)
{
	return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}

void changeGameMode(int gm)
{
	stringstream ss;
	getGame()->state = (gameState)gm;
	ss << "Game mode changed to " << gameStates[(int)getGame()->state];
	getGame()->debug.AddEvent(ss.str());
	if((gameState)gm == choosingShip)
		getGame()->chooseShipMenu.pointer->coords = getGame()->gametype->settingsMenu.pointer->coords;
}

void GoToSettings(int gameType)
{
	changeGameMode(settings);
	switch(gameType)
	{
	case ffa:
		getGame()->gametype = NULL;
		break;
	case ctf:
		getGame()->gametype = new GameType;
		break;
	}
	getGame()->gametype->settingsMenu.pointer->coords = getGame()->menu.pointer->coords;
}

void StartGame(int gameType)
{
	changeGameMode(running);
	getGame()->gametype->OnStart(getGame());
}

void ChangeGameObjective(int points)
{
	getGameType()->changeObjective(points);
}

void ChangeTeamPlayers(int members)
{
	getGameType()->changeTeamPlayers(members);
}

string itos(int i)
{
	static char c[100];
	_itoa_s(i, c, 100, 10);
	return string(c);
}

void MakeNewShip(int i)
{
	getGame()->ChangeShip(getGame()->gametype->RespawnShip(getGame()->temploc, getGame()->tempdir, getGame()->tempvel, renderableType(i), getGame()->pc->number));
	changeGameMode(running);
}