#ifndef DEBUGEVENT_H
#define DEBUGEVENT_H

#include <string>
using namespace std;

class DebugEvent
{
public:
	DebugEvent(string n, int fn);
	~DebugEvent(void);

	string name;
	int frameNum;
};

#endif