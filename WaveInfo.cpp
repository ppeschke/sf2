#include "WaveInfo.h"

WaveInfo::WaveInfo(LPCTSTR filename)
{
	dataOffset = 0;
	dataLength = 0;
	memset(&waveFormat, 0, sizeof(waveFormat));
	load(filename);
}

WaveInfo::WaveInfo(const WaveInfo& c)
{
	waveFormat = c.waveFormat;
	dataOffset = c.dataOffset;
	dataLength = c.dataLength;
}

DWORD WaveInfo::findChunk(HANDLE file, FOURCC cc, BYTE* memBuffer, DWORD sectorAlignment)
{
	DWORD dwChunkId = 0;
	DWORD dwChunkSize = 0;
	DWORD i = 0; //guaranteed to be always aligned with the sectors, except when done searching
	OVERLAPPED overlapped = {0};
	DWORD sectorOffset = 0;
	DWORD bytesRead = 0;

	bool searching = true;
	while(searching)
	{
		sectorOffset = 0;
		overlapped.Offset = i;
		if(ReadFile(file, memBuffer, sectorAlignment, &bytesRead, &overlapped) == FALSE)
			return -1;

		bool needAnotherRead = false;
		while(searching && !needAnotherRead)
		{
			if(8 + sectorOffset > sectorAlignment) //reached the end of our memory buffer
				needAnotherRead = true;
			else if( 8 + sectorOffset > bytesRead ) //reached EOF, and not found a match
				return -1;
			else //looking through the read memory
			{
				dwChunkId = *reinterpret_cast<DWORD*>( memBuffer + sectorOffset );
				dwChunkSize = *reinterpret_cast<DWORD*>( memBuffer + sectorOffset + 4 );

				if(dwChunkId == cc) //found a match
				{
					searching = false;
					i += sectorOffset;
				}
				else //no match found, add to offset
				{
					dwChunkSize += 8; //add offsets of the chunk id, and chunk size data entries
					dwChunkSize += 1;
					dwChunkSize &= 0xfffffffe; //guarantees WORD padding alignment

					if(i == 0 && sectorOffset == 0) //just in case we're at the 'RIFF' chunk; the dwChunkSize here means the entire file size
						sectorOffset += 12;
					else
						sectorOffset += dwChunkSize;
				}
			}
		}

		//if still searching, search the next sector
		if(searching)
		{
			i += sectorAlignment;
		}
	}

	return i;
}

DWORD WaveInfo::readData(HANDLE file, DWORD bytesToRead, DWORD fileOffset, void* destination, BYTE* memBuffer, DWORD sectorAlignment)
{
	if(bytesToRead == 0)
		return 0;

	DWORD totalAmountCopied = 0;
	DWORD copyBeginOffset = fileOffset % sectorAlignment;
	OVERLAPPED overlapped = {0};	//initialize all to 0's
	bool fetchingData = true;
	DWORD pass = 0;
	DWORD dwNumberBytesRead = 0;

	//while fetching data
	while(fetchingData)
	{
		//calculate the sector to read
		overlapped.Offset = fileOffset - (fileOffset % sectorAlignment) + pass * sectorAlignment;

		//read the amount in; if the read failed, return 0
		if(ReadFile( file, memBuffer, sectorAlignment, &dwNumberBytesRead, &overlapped) == FALSE)
			return 0;

		//if the full buffer was not filled (ie. EOF)
		if(dwNumberBytesRead < sectorAlignment)
		{
			//calculate how much can be copied
			DWORD amountToCopy = 0;
			if(dwNumberBytesRead > copyBeginOffset)
				amountToCopy = dwNumberBytesRead - copyBeginOffset;
			if(totalAmountCopied + amountToCopy > bytesToRead)
				amountToCopy = bytesToRead - totalAmountCopied;

			//copy that amount over
			memcpy(((BYTE*)destination) + totalAmountCopied, memBuffer + copyBeginOffset, amountToCopy);

			//add to the total amount copied
			totalAmountCopied += amountToCopy;

			//end the data fetching loop
			fetchingData = false;
		}
		//else
		else
		{
			//calculate how much can be copied
			DWORD amountToCopy = sectorAlignment - copyBeginOffset;
			if(totalAmountCopied + amountToCopy > bytesToRead)
				amountToCopy = bytesToRead - totalAmountCopied;

			//copy that amount over
			memcpy(((BYTE*)destination) + totalAmountCopied, memBuffer + copyBeginOffset, amountToCopy);

			//add to the total amount copied
			totalAmountCopied += amountToCopy;

			//set the copyBeginOffset to 0
			copyBeginOffset = 0;
		}

		//if the total amount equals the bytesToRead, end the fetching data loop
		if(totalAmountCopied == bytesToRead)
			fetchingData = false;

		//increment the pass
		++pass;
	}

	return totalAmountCopied;
}

bool WaveInfo::load(LPCTSTR filename)
{
	memset(&waveFormat, 0, sizeof(waveFormat));
	dataOffset = 0;
	dataLength = 0;

	if(filename == NULL)
		return false;

	//load the file without system cacheing
	HANDLE file = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);

	if(file == INVALID_HANDLE_VALUE)
		return false;		//file failed to open

	//figure out the sector size for reading
	DWORD SectorSize = 0;

	{
		DWORD dw1, dw2, dw3;	//these will expire at the end of this block (clever eh?)
		GetDiskFreeSpace(NULL, &dw1, &SectorSize, &dw2, &dw3);
	}

	//allocate the aligned memory buffer, used in finding and reading the chunks in the file
	BYTE *memBuffer = (BYTE*)_aligned_malloc(SectorSize, SectorSize);
	if(memBuffer == NULL)
	{
		CloseHandle(file);	//clean up
		return false;
	}

	//look for 'RIFF' chunk
	DWORD ChunkOffset = findChunk(file, MAKEFOURCC('R', 'I', 'F', 'F'), memBuffer, SectorSize);
	if(ChunkOffset == -1)
	{
		_aligned_free(memBuffer);	//clean up
		CloseHandle(file);
		return false;
	}

	DWORD riffFormat = 0;
	//inFile.seekg( dwChunkOffset + 8, std::ios::beg );
	//inFile.read( reinterpret_cast<char*>(&riffFormat), sizeof(riffFormat) );
	if(readData(file, sizeof(riffFormat), ChunkOffset + 8, &riffFormat, memBuffer, SectorSize) != sizeof(DWORD))
	{
		_aligned_free(memBuffer);
		CloseHandle(file);
		return false;
	}
	if(MAKEFOURCC('W', 'A', 'V', 'E') != riffFormat)
	{
		_aligned_free(memBuffer);
		CloseHandle(file);
		return false;
	}

	//look for 'fmt ' chunk
	ChunkOffset = findChunk(file, MAKEFOURCC('f', 'm', 't', ' '), memBuffer, SectorSize);
	if(ChunkOffset == -1)
	{
		_aligned_free(memBuffer);
		CloseHandle(file);
		return false;
	}

	//read in first the WAVEFORMATEX structure
	//inFile.seekg( dwChunkOffset + 8, std::ios::beg );
	//inFile.read( reinterpret_cast<char*>(&m_wf.Format), sizeof(m_wf.Format) );
	if(readData(file, sizeof(waveFormat.Format), ChunkOffset + 8, &waveFormat.Format, memBuffer, SectorSize ) != sizeof(waveFormat.Format))
	{
		_aligned_free(memBuffer);	//clean up
		CloseHandle(file);
		return false;
	}
	if(waveFormat.Format.cbSize == (sizeof(waveFormat) - sizeof(waveFormat.Format)))
	{
		//read in whole WAVEFORMATEXTENSIBLE structure
		//inFile.seekg( dwChunkOffset + 8, std::ios::beg );
		//inFile.read( reinterpret_cast<char*>(&m_wf), sizeof(m_wf) );
		if(readData(file, sizeof(waveFormat), ChunkOffset + 8, &waveFormat, memBuffer, SectorSize ) != sizeof(waveFormat))
		{
			_aligned_free(memBuffer);	//clean up
			CloseHandle(file);
			return false;
		}
	}

	//look for 'data' chunk
	ChunkOffset = findChunk(file, MAKEFOURCC('d', 'a', 't', 'a'), memBuffer, SectorSize);
	if(ChunkOffset == -1)
	{
		_aligned_free(memBuffer);	//clean up
		CloseHandle(file);
		return false;
	}

	//set the offset to the wave data, read in length, then return
	dataOffset = ChunkOffset + 8;
	//inFile.seekg( dwChunkOffset + 4, std::ios::beg );
	//inFile.read( reinterpret_cast<char*>(&m_dataLength), 4 );
	if(readData(file, sizeof(dataLength), ChunkOffset + 4, &dataLength, memBuffer, SectorSize ) != sizeof(dataLength))
	{
		_aligned_free(memBuffer);	//clean up
		CloseHandle(file);
		return false;
	}

	_aligned_free(memBuffer);	//final clean up
	CloseHandle(file );

	return true;
}