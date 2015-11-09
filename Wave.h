#ifndef WAVE_H
#define WAVE_H

//wave.h
//by Jay Tennant 3/4/12
//Wave helper class, to load simple wave files
//win32developer.com
//this code provided free, as in public domain; score!

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <xaudio2.h>

enum soundType {pew, launch, explode, thrusters};

class Wave
{
public:
	Wave(LPCWSTR filename, soundType _st);
	~Wave();

	bool Load(LPCWSTR filename);

	WAVEFORMATEX waveFormat;
	XAUDIO2_BUFFER buffer;
	BYTE* waveData;
	soundType sT;
};

#endif