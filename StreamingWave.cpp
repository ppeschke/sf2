#include "StreamingWave.h"

StreamingWave::StreamingWave(LPCTSTR filename) : WaveInfo(NULL)
{
	file = INVALID_HANDLE_VALUE;
	currentReadPass = 0;
	currentReadBuffer = 0;
	isPrepared = false;
	dataBuffer = NULL;
	sectorAlignment = 0;
	bufferBeginOffset = 0;
	memset(xaBuffer, 0, sizeof(xaBuffer));

	//figure the sector alignment
	DWORD dw1, dw2, dw3;
	GetDiskFreeSpace(NULL, &dw1, &sectorAlignment, &dw2, &dw3);

	//allocate the buffers
	dataBuffer = (BYTE*)_aligned_malloc(STREAMINGWAVE_BUFFER_COUNT * STREAMINGWAVE_BUFFER_SIZE + sectorAlignment, sectorAlignment);
	memset(dataBuffer, 0, STREAMINGWAVE_BUFFER_COUNT * STREAMINGWAVE_BUFFER_SIZE + sectorAlignment);

	load(filename);
}

StreamingWave::StreamingWave(const StreamingWave& c) : WaveInfo(c)
{
	file = c.file;
	currentReadPass = c.currentReadPass;
	currentReadBuffer = c.currentReadBuffer;
	isPrepared = c.isPrepared;
	dataBuffer = NULL;
	sectorAlignment = c.sectorAlignment;
	bufferBeginOffset = c.bufferBeginOffset;

	if(sectorAlignment == 0)
	{
		//figure the sector alignment
		DWORD dw1, dw2, dw3;	//we don't care about these, they die at the end of this block
		GetDiskFreeSpace( NULL, &dw1, &sectorAlignment, &dw2, &dw3 );
	}

	//allocate the buffers
	dataBuffer = (BYTE*)_aligned_malloc(STREAMINGWAVE_BUFFER_COUNT * STREAMINGWAVE_BUFFER_SIZE + sectorAlignment, sectorAlignment);
	memset(dataBuffer, 0, STREAMINGWAVE_BUFFER_COUNT * STREAMINGWAVE_BUFFER_SIZE + sectorAlignment);

	memcpy(dataBuffer, c.dataBuffer, STREAMINGWAVE_BUFFER_COUNT * STREAMINGWAVE_BUFFER_SIZE + sectorAlignment);
	memcpy(xaBuffer, c.xaBuffer, sizeof(xaBuffer));
	for(int i = 0; i < STREAMINGWAVE_BUFFER_COUNT; ++i)
		xaBuffer[i].pAudioData = dataBuffer + bufferBeginOffset + i * STREAMINGWAVE_BUFFER_SIZE;
}

StreamingWave::~StreamingWave()
{
	close();
	if(dataBuffer != NULL )
		_aligned_free(dataBuffer);
	dataBuffer = NULL;
}

bool StreamingWave::load(LPCTSTR filename)
{
	close();
	//test if the data can be loaded
	if(!WaveInfo::load(filename))
		return false;

	//figure the offset for the wave data in allocated memory
	bufferBeginOffset = dataOffset % sectorAlignment;

	//open the file
	file = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);
	if(file == INVALID_HANDLE_VALUE)
		return false;

	//set the xaudio2 buffer struct to refer to appropriate buffer starting points (but leave size of the data as 0)
	for(int i = 0; i < STREAMINGWAVE_BUFFER_COUNT; ++i)
		xaBuffer[i].pAudioData = dataBuffer + bufferBeginOffset + i * STREAMINGWAVE_BUFFER_SIZE;

	return true;
}

//closes the file stream, resetting this object's state
void StreamingWave::close()
{
	if(file != INVALID_HANDLE_VALUE)
		CloseHandle(file);
	file = INVALID_HANDLE_VALUE;

	bufferBeginOffset = 0;
	memset(xaBuffer, 0, sizeof(xaBuffer));
	memset(dataBuffer, 0, STREAMINGWAVE_BUFFER_COUNT * STREAMINGWAVE_BUFFER_SIZE + sectorAlignment);
	isPrepared = false;
	currentReadBuffer = 0;
	currentReadPass = 0;

	WaveInfo::load(NULL);
}

void StreamingWave::swap()
{
	currentReadBuffer = (currentReadBuffer + 1) % STREAMINGWAVE_BUFFER_COUNT;
	isPrepared = false;
}

const XAUDIO2_BUFFER* StreamingWave::buffer() const
{
	return &xaBuffer[(currentReadBuffer + STREAMINGWAVE_BUFFER_COUNT - 1) % STREAMINGWAVE_BUFFER_COUNT];
}

DWORD StreamingWave::prepare()
{
	//validation check
	if(file == INVALID_HANDLE_VALUE)
	{
		xaBuffer[currentReadBuffer].AudioBytes = 0;
		xaBuffer[currentReadBuffer].Flags = XAUDIO2_END_OF_STREAM;
		return PR_FAILURE;
	}

	//are we already prepared?
	if(isPrepared)
		return PR_SUCCESS;

	//figure the offset of the file pointer
	OVERLAPPED overlapped = {0};
	overlapped.Offset = dataOffset - bufferBeginOffset + STREAMINGWAVE_BUFFER_SIZE * currentReadPass;

	//preliminary end-of-data check
	if(overlapped.Offset + bufferBeginOffset > dataLength + dataOffset)
	{
		xaBuffer[currentReadBuffer].AudioBytes = 0;
		xaBuffer[currentReadBuffer].Flags = XAUDIO2_END_OF_STREAM;
		isPrepared = true;
		return PR_EOF;
	}

	//read in data from file
	DWORD NumBytesRead = 0;
	if(ReadFile(file, dataBuffer + STREAMINGWAVE_BUFFER_SIZE * currentReadBuffer, STREAMINGWAVE_BUFFER_SIZE + sectorAlignment, &NumBytesRead, &overlapped) == FALSE)
	{
		xaBuffer[currentReadBuffer].AudioBytes = 0;
		xaBuffer[currentReadBuffer].Flags = XAUDIO2_END_OF_STREAM;
		return PR_FAILURE;
	}

	//force NumBytesRead to be less than the actual amount read if reading past the end of the data chunk
	if(NumBytesRead + STREAMINGWAVE_BUFFER_SIZE * currentReadPass > dataLength)
	{
		if(STREAMINGWAVE_BUFFER_SIZE * currentReadPass <= dataLength)
			NumBytesRead = min(NumBytesRead, dataLength - STREAMINGWAVE_BUFFER_SIZE * currentReadPass); //bytes read are from overlapping file chunks
		else
			NumBytesRead = 0; //none of the bytes are from the correct data chunk; this should never happen due to the preliminary end-of-data check, unless the file was wrong
	}

	//end-of-file/data check
	if(NumBytesRead < STREAMINGWAVE_BUFFER_SIZE + sectorAlignment)
	{
		//check for case where less than the sectorAlignment amount of data is still available in the file
		//of course, only do something if there isn't that amount of data left
		if(NumBytesRead < bufferBeginOffset)
		{//no valid data at all; this shouldn't happen since the preliminary end-of-data check happened already, unless the file was wrong
			xaBuffer[currentReadBuffer].AudioBytes = 0;
			xaBuffer[currentReadBuffer].Flags = XAUDIO2_END_OF_STREAM;
			isPrepared = true;

			//increment the current read pass
			++currentReadPass;
			return PR_EOF;
		}
		else if(NumBytesRead - bufferBeginOffset <= STREAMINGWAVE_BUFFER_SIZE)
		{//some valid data; this should always happen for the end-of-file and end-of-data conditions
			xaBuffer[currentReadBuffer].AudioBytes = NumBytesRead - bufferBeginOffset; //do not include the data offset as valid data
			xaBuffer[currentReadBuffer].Flags = XAUDIO2_END_OF_STREAM;
			isPrepared = true;

			//increment the current read pass
			++currentReadPass;
			return PR_EOF;
		}
	}

	//set the amount of data available;
	//this should always be STREAMINGWAVE_BUFFER_SIZE, unless one of the previous conditions (end-of-file, end-of-data) were met
	xaBuffer[currentReadBuffer].AudioBytes = STREAMINGWAVE_BUFFER_SIZE;
	xaBuffer[currentReadBuffer].Flags = 0;
	isPrepared = true;

	//increment the current read pass
	++currentReadPass;

	//return success
	return PR_SUCCESS;
}