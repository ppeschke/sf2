#ifndef STREAMINGWAVE_H
#define STREAMINGWAVE_H

#define WIN32_LEAN_AND_MEAN
#include "WaveInfo.h"

#define STREAMINGWAVE_BUFFER_SIZE 65536	//should always be a power of 2
										//to guarentee a multiple of the sector size (most are at or below 4096)
#define STREAMINGWAVE_BUFFER_COUNT 3	//should never be less than 3

class StreamingWave : public WaveInfo
{
public:
	StreamingWave(LPCTSTR filename = NULL);
	StreamingWave(const StreamingWave& c);
	~StreamingWave();

	bool load(LPCTSTR filename);	//loads the file for streaming wave data
	void close();
	void swap();	//swaps the presentation buffer to the next one
	const XAUDIO2_BUFFER* buffer() const;	//gets the current buffer

	//resets the file pointer to the beginning of the wave data;
	//this will not wipe out buffers that have been prepared, so it is safe to call
	//after a call to prepare() has returned PR_EOF, and before a call to swap() has
	//been made to present the prepared buffer
	void resetFile()	//inline
	{currentReadPass = 0;}

	//prepares the next buffer for presentation;
	//returns PR_SUCCESS on success,
	//PR_FAILURE on failure,
	//and PR_EOF when the end of the data has been reached
	DWORD prepare();

	enum PREPARE_RESULT {PR_SUCCESS, PR_FAILURE, PR_EOF};	//prepare needs these to function correctly


	//members
	HANDLE file;
	DWORD currentReadPass;		//the current pass for reading
								//this number multiplied by STREAMINGWAVE_BUFFER_SIZE and adding getDataOffset() represents the file position
	DWORD currentReadBuffer;	//the current buffer used for reading from file; the presentation buffer is the one right before this
	bool isPrepared;			//whether the buffer is prepared for the swap
	BYTE *dataBuffer;			//the wave buffers; the size is STREAMINGWAVE_BUFFER_COUNT * STREAMINGWAVE_BUFFER_SIZE + m_sectorAlignment

	//the xaudio2 buffer information
	XAUDIO2_BUFFER xaBuffer[STREAMINGWAVE_BUFFER_COUNT];
	DWORD sectorAlignment;		//the sector alignment for reading
								//this value is added to the entire buffer's size for sector-aligned reading and reference
	DWORD bufferBeginOffset;	//the starting offset for each buffer (when the file reads are offset by an amount)
};

#endif