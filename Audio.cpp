#include "Global.h"
#include "Wave.h"

void LoadSound(LPCWSTR filename, soundType sT);

void LoadSounds(Game* thegame)
{
	InitXAudio(thegame);
	LoadSound(L"sounds\\pew.wav", pew);
	LoadSound(L"sounds\\launch.wav", launch);
	LoadSound(L"sounds\\explode.wav", explode);
	LoadSound(L"sounds\\pinknoise.wav", thrusters);
}

void LoadSound(LPCWSTR filename, soundType sT)
{
	Wave* w = new Wave(filename, sT);
	LoadSoundFromFile(filename, w->waveFormat, w->buffer);
	getGame()->sounds.insert(getGame()->sounds.end(), w);
}

void PlayThatSound(Wave* w)
{
	Play(w);
}

void LoopSound(Wave* w)
{
	_Loop(L"sounds\\playing.wav");
}