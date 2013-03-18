#include "Wave.h"
#include "Global.h"
#include <fstream>
using namespace std;

Wave::Wave(LPCWSTR filename, soundType _st)
{
	waveData = NULL;
	ZeroMemory(&waveFormat, sizeof(waveFormat));
	ZeroMemory(&buffer, sizeof(buffer));
	sT = _st;

	Load(filename);
}

/*Wave(const Wave& c) : waveData(NULL) {
		waveFormat = c.waveFormat;
		buffer = c.buffer;
		if(c.waveData)
		{
			waveData = new BYTE[buffer.AudioBytes];
			memcpy( waveData, c.waveData, buffer.AudioBytes );
			buffer.pAudioData = waveData;
		}
	}*/

Wave::~Wave()
{
	if(waveData)
		delete [] waveData;
	waveData = NULL;
}

bool Wave::Load(LPCWSTR filename)
{
	LoadSoundFromFile(filename, this->waveFormat, this->buffer);
	/*if(filename == NULL)
		return false;

	ifstream fin(filename, ios::binary | ios::in);
	if(fin.bad())
		return false;
	
	DWORD dwChunkId = 0, dwFileSize = 0, dwChunkSize = 0, dwExtra = 0;

	//look for 'RIFF' chunk identifier
	fin.seekg(0, std::ios::beg);
	fin.read(reinterpret_cast<char*>(&dwChunkId), sizeof(dwChunkId));
	if(dwChunkId != 'FFIR')
	{
		fin.close();
		return false;
	}
	fin.seekg(4, ios::beg); //get file size
	fin.read(reinterpret_cast<char*>(&dwFileSize), sizeof(dwFileSize));
	if(dwFileSize <= 16)
	{
		fin.close();
		return false;
	}
	fin.seekg(8, ios::beg); //get file format
	fin.read(reinterpret_cast<char*>(&dwExtra), sizeof(dwExtra));
	if(dwExtra != 'EVAW')
	{
		fin.close();
		return false;
	}

	//look for 'fmt ' chunk id
	bool bFilledFormat = false;
	for(unsigned int i = 12; i < dwFileSize; )
	{
		fin.seekg(i, std::ios::beg);
		fin.read(reinterpret_cast<char*>(&dwChunkId), sizeof(dwChunkId));
		fin.seekg(i + 4, std::ios::beg);
		fin.read(reinterpret_cast<char*>(&dwChunkSize), sizeof(dwChunkSize));
		if(dwChunkId == ' tmf')
		{
			//I don't know what I was thinking with the following code, but I
			//never did solve it back 6 months, and didn't touch it since; oh well... :S

			//switch(dwChunkSize)
			//{
			//case sizeof(WAVEFORMATEX):
			//	{
			//		fin.seekg(i + 8, std::ios::beg);
			//		fin.read(reinterpret_cast<char*>(&waveFormat), sizeof(waveFormat));
			//	}
			//	break;
			//case sizeof(WAVEFORMATEXTENSIBLE):
			//	{
			//		WAVEFORMATEXTENSIBLE wfe;
			//		fin.seekg(i + 8, std::ios::beg);
			//		fin.read(reinterpret_cast<char*>(&wfe), sizeof(wfe));
			//		waveFormat = wfe.Format;
			//	}
			//	break;
			//default:
			//	fin.close();
			//	return;
			//}
			fin.seekg(i + 8, std::ios::beg);
			fin.read(reinterpret_cast<char*>(&waveFormat), sizeof(waveFormat));
			bFilledFormat = true;
			break;
		}
		dwChunkSize += 8; //add offsets of the chunk id, and chunk size data entries
		dwChunkSize += 1;
		dwChunkSize &= 0xfffffffe; //guarantees WORD padding alignment
		i += dwChunkSize;
	}
	if(!bFilledFormat)
	{
		fin.close();
		return false;
	}

	//look for 'data' chunk id
	bool bFilledData = false;
	for(unsigned int i = 12; i < dwFileSize; )
	{
		fin.seekg(i, std::ios::beg);
		fin.read(reinterpret_cast<char*>(&dwChunkId), sizeof(dwChunkId));
		fin.seekg(i + 4, std::ios::beg);
		fin.read(reinterpret_cast<char*>(&dwChunkSize), sizeof(dwChunkSize));
		if(dwChunkId == 'atad')
		{
			waveData = new BYTE[dwChunkSize];
			fin.seekg(i + 8, std::ios::beg);
			fin.read(reinterpret_cast<char*>(waveData), dwChunkSize);
			buffer.AudioBytes = dwChunkSize;
			buffer.pAudioData = waveData;
			buffer.PlayBegin = 0;
			buffer.PlayLength = 0;
			bFilledData = true;
			break;
		}
		dwChunkSize += 8; //add offsets of the chunk id, and chunk size data entries
		dwChunkSize += 1;
		dwChunkSize &= 0xfffffffe; //guarantees WORD padding alignment
		i += dwChunkSize;
	}
	if(!bFilledData)
	{
		fin.close();
		return false;
	}

	fin.close();*/
	return true;
}