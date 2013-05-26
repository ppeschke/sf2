#ifndef	DEBUGTOOLS_H
#define DEBUGTOOLS_H

#include <fstream>
#include <string>
#include <vector>
using namespace std;
#include "_Process.h"

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

	_Process* current;
	vector<_Process*> frames;

	debugLevel level;
	unsigned int frameCount;
	unsigned int threshold;

private:
	void outputHeader(ofstream&);
	void outputFooter(ofstream&);
};

#endif
