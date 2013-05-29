#ifndef	DEBUGTOOLS_H
#define DEBUGTOOLS_H

#include <fstream>
#include <string>
#include <vector>
using namespace std;
#include "_Process.h"
#include "DebugEvent.h"

class DebugTools
{
public:
	DebugTools(unsigned int frameThreshold, debugLevel dl = warning);
	~DebugTools(void);

	void SelectDebugLevel(debugLevel dl);
	void StartFrame();
	void EndFrame();
	void EnterSubProcess(string s, unsigned int thresh);
	void ExitSubProcess();
	void Output();
	void CleanupProcess(_Process* p);
	void CleanupChildren(_Process* p);
	bool FrameHasEvent(int frameNumber);
	void AddEvent(string name);

	_Process* current;
	vector<_Process*> frames;
	vector<DebugEvent*> events;

	debugLevel level;
	unsigned int frameCount;
	unsigned int threshold;
};

#endif
