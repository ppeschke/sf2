#include "Global.h"
#include "StreamingWave.h"

#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif

const unsigned int MAX_VOICES = 30;

HRESULT ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset);
HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition);
unsigned int getNextSourceVoice();

HANDLE QuitEvent;
HANDLE StreamingVoiceThread;
IXAudio2* XAudio2 = NULL;
IXAudio2MasteringVoice* MasterVoice = NULL;
IXAudio2SourceVoice* SourceVoices[MAX_VOICES];
IXAudio2SourceVoice* BackgroundMusic = NULL;

struct StreamingVoiceCallback : public IXAudio2VoiceCallback
{
public:
	HANDLE BufferEndEvent;

	StreamingVoiceCallback() : BufferEndEvent(CreateEvent(NULL, TRUE, FALSE, NULL))
	{}
	virtual ~StreamingVoiceCallback()
	{
		CloseHandle(BufferEndEvent);
	}

	//overrides
    STDMETHOD_(void, OnVoiceProcessingPassStart)(UINT32 bytesRequired)
    {
    }
    STDMETHOD_(void, OnVoiceProcessingPassEnd)()
    {
    }
    STDMETHOD_(void, OnStreamEnd)()
    {
    }
    STDMETHOD_(void, OnBufferStart)(void* pContext)
    {
    }
    STDMETHOD_(void, OnBufferEnd)(void* pContext)	//this is the only method that we overload really
    {
        SetEvent(BufferEndEvent);
    }
    STDMETHOD_(void, OnLoopEnd)(void* pContext)
    {
    }
    STDMETHOD_(void, OnVoiceError)(void* pContext, HRESULT error)
    {
    }
};

//the context to send to the StreamProc
struct StreamContext
{
	IXAudio2SourceVoice** pVoice;	//the source voice that is created on the thread
	LPCTSTR filename;				//name of the file to stream
	HANDLE VoiceLoadEvent;			//lets us know the thread is set up for streaming, or encountered an error
};

//the streaming thread
DWORD WINAPI StreamProc(LPVOID pContext);

void InitXAudio(Game* thegame)
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	HRESULT hr = XAudio2Create(&XAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if(FAILED(hr))
	    exit(EXIT_FAILURE);

	hr = XAudio2->CreateMasteringVoice(&MasterVoice);
	if(FAILED(hr))
		exit(EXIT_FAILURE);
}

void CloseXAudio()
{
	SetEvent(QuitEvent);
	WaitForSingleObject(StreamingVoiceThread, INFINITE);
	//close all handles we opened
	CloseHandle(StreamingVoiceThread);
	CloseHandle(QuitEvent);

	XAudio2->Release();
	CoUninitialize();
}

HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition)
{
    HRESULT hr = S_OK;
    if(INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
        return HRESULT_FROM_WIN32(GetLastError());

    DWORD dwChunkType;
    DWORD dwChunkDataSize;
    DWORD dwRIFFDataSize = 0;
    DWORD dwFileType;
    DWORD bytesRead = 0;
    DWORD dwOffset = 0;

    while (hr == S_OK)
    {
        DWORD dwRead;
        if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
            hr = HRESULT_FROM_WIN32(GetLastError());

        if(ReadFile( hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
            hr = HRESULT_FROM_WIN32(GetLastError());

        switch (dwChunkType)
        {
        case fourccRIFF:
            dwRIFFDataSize = dwChunkDataSize;
            dwChunkDataSize = 4;
            if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL ) == 0)
                hr = HRESULT_FROM_WIN32(GetLastError());
            break;

        default:
            if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
            return HRESULT_FROM_WIN32(GetLastError());            
        }

        dwOffset += sizeof(DWORD) * 2;
        
        if(dwChunkType == fourcc)
        {
            dwChunkSize = dwChunkDataSize;
            dwChunkDataPosition = dwOffset;
            return S_OK;
        }

        dwOffset += dwChunkDataSize;
        
        if(bytesRead >= dwRIFFDataSize)
			return S_FALSE;

    }

    return S_OK;
}

HRESULT ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset)
{
    HRESULT hr = S_OK;
    if(SetFilePointer( hFile, bufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
        return HRESULT_FROM_WIN32(GetLastError());
    DWORD dwRead;
    if(ReadFile( hFile, buffer, buffersize, &dwRead, NULL) == 0)
        hr = HRESULT_FROM_WIN32(GetLastError());
    return hr;
}

void LoadSoundFromFile(LPCWSTR filename, WAVEFORMATEX& waveFormat, XAUDIO2_BUFFER& buffer)
{
	// Open the file
	HANDLE hFile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if(INVALID_HANDLE_VALUE == hFile)
		exit(EXIT_FAILURE);

	if(INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		exit(EXIT_FAILURE);

	//locate the riff chunk and check the filetype
	DWORD dwChunkSize;
	DWORD dwChunkPosition;
	//check the file type, should be fourccWAVE or 'XWMA'
	FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
	DWORD filetype;
	ReadChunkData(hFile,&filetype, sizeof(DWORD), dwChunkPosition);
	if(filetype != fourccWAVE)
		exit(EXIT_FAILURE);

	//locate the "fmt" chunk and copy it into a WAVEFMTEXTENSIBLE structure (wfx)
	//WAVEFORMATEXTENSIBLE wfx = {0};
	FindChunk(hFile,fourccFMT, dwChunkSize, dwChunkPosition);
	ReadChunkData(hFile, &waveFormat, dwChunkSize, dwChunkPosition);

	//Locate the 'data' chunk, and read its contents into a buffer
	//fill out the audio data buffer with the contents of the fourccDATA chunk
	FindChunk(hFile,fourccDATA, dwChunkSize, dwChunkPosition);
	BYTE * pDataBuffer = new BYTE[dwChunkSize];
	ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

	//Populate an XAUDIO2_BUFFER structure
	buffer.AudioBytes = dwChunkSize;  //buffer containing audio data
	buffer.pAudioData = pDataBuffer;  //size of the audio buffer in bytes
	buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer
}

void Play(Wave* w)
{
	unsigned int nextVoice = getNextSourceVoice();
	HRESULT hr;

	if(!SourceVoices[nextVoice])
		hr = XAudio2->CreateSourceVoice(&SourceVoices[nextVoice], (WAVEFORMATEX*)(&w->waveFormat));

	hr = SourceVoices[nextVoice]->Stop();				//stop any previous audio processing
	hr = SourceVoices[nextVoice]->FlushSourceBuffers();	//clear the source buffers
	hr = SourceVoices[nextVoice]->Start();				//start current audio processing

	//Submit the XAUDIO2_BUFFER to the source voice using the function SubmitSourceBuffer
	hr = SourceVoices[nextVoice]->SubmitSourceBuffer(&w->buffer);
}

unsigned int getNextSourceVoice()
{
	static unsigned int nextVoice = 0;
	if(nextVoice == MAX_VOICES)
		nextVoice = 0;
	return nextVoice++;	//add 1 when (after) returning
}

void _Loop(LPCTSTR filename)
{
	StreamContext streamContext = {NULL, filename, CreateEvent(NULL, FALSE, FALSE, NULL)};
	//create the abort event to end playing thread
	QuitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	//create the streaming voice thread
	DWORD ThreadId = 0;
	StreamingVoiceThread = CreateThread(NULL, 0, StreamProc, &streamContext, 0, &ThreadId);
	if(StreamingVoiceThread == NULL)
	{
		CloseHandle(QuitEvent);
		XAudio2->Release();
		CoUninitialize();
		exit(EXIT_FAILURE);
	}

	//wait for the streaming voice thread to signal that it's either
	//prepared the streaming, or that it's encountered an error
	WaitForSingleObject(streamContext.VoiceLoadEvent, INFINITE);
	if(streamContext.pVoice == NULL || (*streamContext.pVoice) == NULL)
	{
		SetEvent(QuitEvent);
		WaitForSingleObject(StreamingVoiceThread, INFINITE);

		CloseHandle(StreamingVoiceThread);
		CloseHandle(QuitEvent );
		XAudio2->Release();
		CoUninitialize();
		exit(EXIT_FAILURE);
	}

	//starting up the shields!
	if(streamContext.pVoice != NULL)
		(*streamContext.pVoice)->Start();

}

//the streaming thread procedure
DWORD WINAPI StreamProc(LPVOID pContext)
{
	//required by XAudio2
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	if(pContext == NULL)
	{
		CoUninitialize();
		return -1;
	}

	StreamContext* sc = (StreamContext*)pContext;

	//instantiate the voice's callback class
	StreamingVoiceCallback callback;

	//load a file for streaming, non-buffered disk reads (no system cacheing)
	StreamingWave inFile;
	if(!inFile.load(sc->filename))
	{
		SetEvent(sc->VoiceLoadEvent);
		CoUninitialize();
		return -3;
	}

	//create the voice
	IXAudio2SourceVoice* source = NULL;
	if(FAILED(XAudio2->CreateSourceVoice(&source, &inFile.waveFormat.Format, 0, 2.0f, &callback)))
	{
		SetEvent(sc->VoiceLoadEvent);
		CoUninitialize();
		return -5;
	}

	//fill and queue the maximum number of buffers (except the one needed for reading new wave data)
	bool somethingsWrong = false;
	XAUDIO2_VOICE_STATE voiceState = {0};
	source->GetState(&voiceState);
	while(voiceState.BuffersQueued < STREAMINGWAVE_BUFFER_COUNT - 1 && !somethingsWrong)
	{
		//read and fill the next buffer to present
		switch(inFile.prepare())
		{
		case StreamingWave::PR_EOF:
			//if end of file, loop the file read
			inFile.resetFile(); //intentionally fall-through to loop sound
		case StreamingWave::PR_SUCCESS:
			//present the next available buffer
			inFile.swap();
			//submit another buffer
			source->SubmitSourceBuffer(inFile.buffer());
			source->GetState(&voiceState);
			break;
		case StreamingWave::PR_FAILURE:
			somethingsWrong = true;
			break;
		}
	}

	//return the created voice through the context pointer
	sc->pVoice = &source;

	//signal that the voice has prepared for streaming, and ready to start
	SetEvent(sc->VoiceLoadEvent);

	//group the events for the Wait function
	HANDLE Events[2] = {callback.BufferEndEvent, QuitEvent};

	bool quitting = false;
	while(!quitting)
	{
		//wait until either the source voice is ready for another buffer, or the abort signal is set
		DWORD eventFired = WaitForMultipleObjects(2, Events, FALSE, INFINITE);
		switch(eventFired)
		{
		case 0: //buffer ended event for source voice
			//reset the event manually
			ResetEvent(Events[0]);

			//make sure there's a full number of buffers
			source->GetState(&voiceState);
			while(voiceState.BuffersQueued < STREAMINGWAVE_BUFFER_COUNT - 1 && !somethingsWrong)
			{
				//read and fill the next buffer to present
				switch(inFile.prepare())
				{
				case StreamingWave::PR_EOF:
					//if end of file, loop the file read
					inFile.resetFile();	//intentionally fall-through to loop sound
				case StreamingWave::PR_SUCCESS:
					//present the next available buffer
					inFile.swap();
					//submit another buffer
					source->SubmitSourceBuffer(inFile.buffer());
					source->GetState(&voiceState);
					break;
				case StreamingWave::PR_FAILURE:
					somethingsWrong = true;
					break;
				}
			}
			break;
		default: //something's wrong...
			quitting = true;
		}
	}

	//stop and destroy the voice
	source->Stop();
	source->FlushSourceBuffers();
	source->DestroyVoice();

	//close the streaming wave file;
	//this is done automatically in the class destructor,
	//so this is redundant
	inFile.close();

	//cleanup
	CoUninitialize();
	return 0;
}