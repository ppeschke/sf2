#include "DebugTools.h"
#include <sstream>
using namespace std;

DebugTools::DebugTools(unsigned int frameThreshold, debugLevel dl)
{
	current = NULL;
	frameCount = 0;
	threshold = frameThreshold;
	SelectDebugLevel(dl);
}

DebugTools::~DebugTools(void)
{
	Output();
	//delete frames
	for(vector<_Process*>::iterator index = frames.begin(); index != frames.end(); ++index)
		CleanupProcess(*index);
}

void DebugTools::StartFrame()
{
	stringstream ss;
	ss << "Frame No. " << frameCount + 1;
	current = new _Process(NULL, ss.str(), threshold);
	frames.insert(frames.end(), current);
}

void DebugTools::SelectDebugLevel(debugLevel dl)
{
	level = dl;
}

void DebugTools::Output()
{
	ofstream log("debuglog.csv");
	log << frameCount << " frames recorded" << endl;
	for(vector<_Process*>::iterator index = frames.begin(); index != frames.end(); ++index)
	{
		(*index)->Output(log, level);
	}
	
	log.close();
}

void DebugTools::EnterSubProcess(string s, unsigned int thresh)
{
	_Process* temp;
	temp = current;
	current = new _Process(current, s, thresh);
	temp->children.insert(temp->children.end(), current);
}

void DebugTools::ExitSubProcess()
{
	current->timer = GetTickCount() - current->timer;
	current = current->parent;
}

void DebugTools::CleanupChildren(_Process* p)
{
	if(p->children.size())
	{
		for(vector<_Process*>::iterator index = p->children.begin(); index != p->children.end(); ++index)
		{
			CleanupChildren(*index);
		}
	}
}

void DebugTools::CleanupProcess(_Process* p)
{
	CleanupChildren(p);
	delete p;
}

//assumes all running sub-processes stop here
void DebugTools::EndFrame()
{
	while(current->parent != NULL)
	{
		ExitSubProcess();
	}
	current->timer = GetTickCount() - current->timer;
	++frameCount;
}