#ifndef WAVEINFO_H
#define WAVEINFO_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <xaudio2.h>

class WaveInfo
{
public:
	WaveInfo(LPCTSTR filename);
	WaveInfo(const WaveInfo& c);

	DWORD findChunk(HANDLE file, FOURCC cc, BYTE* memBuffer, DWORD sectorAlignment);
	DWORD readData(HANDLE file, DWORD bytesToRead, DWORD fileOffset, void* destination, BYTE* memBuffer, DWORD sectorAlignment);
	bool load(LPCTSTR filename);

	//inline function
	bool isExtensible() const
	{return (waveFormat.Format.cbSize > 0);}

	WAVEFORMATEXTENSIBLE waveFormat;
	DWORD dataOffset;
	DWORD dataLength;
};

#endif