#ifndef _PROCESS_H
#define _PROCESS_H

#include <vector>
#include <fstream>
#include <string>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
using namespace std;

enum debugLevel {all, warning};

class _Process
{
public:
	_Process(_Process* _parent, string _name, DWORD _threshold, int framenum);
	~_Process(void);

	void Output(ofstream& stream, string parent, debugLevel dl, unsigned int level = 0U, bool force = false);	//0U is: 0 unsigned int
	bool BreaksThreshold();

	_Process* parent;
	string name;
	DWORD threshold;
	DWORD timer;
	int framenumber;
	vector<_Process*> children;
};

#endif
