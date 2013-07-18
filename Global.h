#include <windows.h>
#include <windowsx.h>
#include <typeinfo>
#include <d3d9.h>
#include <d3dx9.h>
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif
#include <dinput.h>
#include <string>
#include "GameWindow.h"
#include "GameType.h"
#include "Particle.h"
#include "Game.h"

class Sprite;
//class Sound;
struct XAUDIO2_BUFFER;

//WinMain.cpp
bool HandleMessages();
extern inline Game* getGame();
int randomNumber(int low, int high);
float randomFloat(float low, float high);
float processNumber(string s);
float Distance(const Vec2D& a, const Vec2D& b);
GameType* getGameType();
void changeGameMode(int gm);
void GoToSettings(int gm);
void StartGame(int unused);
void ChangeGameObjective(int points);
void ChangeTeamPlayers(int members);
string itos(int i);
void MakeNewShip(int i);

//Loop.cpp
void Loop(Game* thegame);

//Render.cpp
void LoadGraphics(Game* thegame);
void Render(Game* thegame);
void UnloadGraphics(Game* thegame);

//Audio.cpp
void InitSound(Game* thegame);
void LoadSounds(Game* thegame);
void LoopSound(Wave* w);
void PlayThatSound(Wave* w);

//XAudio.cpp
void InitXAudio(Game* thegame);
void LoadSoundFromFile(LPCWSTR filename, WAVEFORMATEX& waveFormat, XAUDIO2_BUFFER& buffer);
void Play(Wave* w);
void _Loop(LPCTSTR filename);
void CloseXAudio();

//direct3D.cpp
void StartRender();
void EndRender();
void SetCamera();
void AdjustCamera(float x, float y, Game* thegame);
void InitDirect3D(Game* thegame);
void DrawMesh(Mesh* mesh, Vec2D* loc, Vec2D* dir);
void DrawMeshPartWithRotations(sf2::Polygon* poly, Vec2D* loc, Vec2D* dir, Vec2D* rotationPoint, float angle);
void DrawParticleTexture(Texture* t, D3DXVECTOR3 position);
void drawText(const char* text, int x, int y, int r, int g, int b);
void drawBigText(const char* text, int x, int y, int r, int g, int b);
void CloseDirect3D();
void Draw2DCircle(float x, float y, float radius, D3DCOLOR color);
void Draw2DLine(float x1, float y1, float x2, float y2, D3DCOLOR color);
void DrawRect(float x, float y, float length, float width, D3DCOLOR color);
void LoadToVRAM(Mesh* mesh);
void LoadTextureFromFile(LPCWSTR filename, LPDIRECT3DVERTEXBUFFER9& t_vertex, LPDIRECT3DTEXTURE9& texture, int r, int g, int b);
void LoadSpriteFromFile(LPCWSTR filename, Sprite* s);
void DrawBlendedParticles(Particle* p, int numParticles, Texture* t);
void StartSpriteRender();
void EndSpriteRender();
void DrawSprite(Sprite* s, int x, int y);
void DrawSpriteTransparent(Sprite* s, int x, int y, int a);
Vec2D GetScreenCoords(Vec2D vec);
void get3DScreenEdges(Vec2D& topLeft, Vec2D& bottomRight);
Mesh* CreateArena(float radius, Color color);

//directInput.cpp
void InitDirectInput(HINSTANCE, Game*);
void GetKeys(BYTE* KeyState);
void GetMouse(DIMOUSESTATE* MouseState);
void CloseDirectInput();

//Input.cpp
void Input(Game* thegame);

//Init.cpp
void Menu(Game* thegame);
void SplashScreen(Game* thegame);

//SpecialAbilities.cpp
void SpawnWarpField(Ship*);
void Cloak(Ship* s);
void Uncloak(Ship* s);