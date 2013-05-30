#include "DebugTools.h"
#include <sstream>
using namespace std;

DebugTools::DebugTools(unsigned int frameThreshold, debugLevel dl)
{
	current = NULL;
	frameCount = 1;
	threshold = frameThreshold;
	SelectDebugLevel(dl);
}

DebugTools::~DebugTools(void)
{
	Output();
	//delete frames
	for(vector<_Process*>::iterator index = frames.begin(); index != frames.end(); ++index)
		CleanupProcess(*index);
	for(vector<DebugEvent*>::iterator index = events.begin(); index != events.end(); ++index)
		delete (*index);
	events.clear();
}

void DebugTools::StartFrame()
{
	stringstream ss;
	ss << "Frame No. " << frameCount;
	current = new _Process(NULL, ss.str(), threshold, this->frameCount);
	frames.insert(frames.end(), current);
}

void DebugTools::SelectDebugLevel(debugLevel dl)
{
	level = dl;
}

void DebugTools::Output()
{
	ofstream log("processes.js");
	ofstream eventlog("events.js");
	for(vector<DebugEvent*>::iterator index = events.begin(); index != events.end(); ++index)
	{
		eventlog << "_Events.push(new Event(\"" << (*index)->name << "\", " << (*index)->frameNum << "));" << endl;
	}
	bool force = false;
	int framecount = 1;
	for(vector<_Process*>::iterator index = frames.begin(); index != frames.end(); ++index)
	{
		if(FrameHasEvent(framecount))
		{
			force = true;
		}
		if((*index)->BreaksThreshold() || this->level == all)
			force = true;
		(*index)->Output(log, "", level, 0, force);
		force = false;
		++framecount;
	}
	log.close();
	eventlog.close();
}

void DebugTools::EnterSubProcess(string s, unsigned int thresh)
{
	_Process* temp;
	temp = current;
	current = new _Process(current, s, thresh, this->frameCount);
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

bool DebugTools::FrameHasEvent(int frameNumber)
{
	for(vector<DebugEvent*>::iterator index = events.begin(); index != events.end(); ++index)
	{
		if((*index)->frameNum == frameNumber)
			return true;
	}
	return false;
}

void DebugTools::AddEvent(string name)
{
	events.insert(events.end(), new DebugEvent(name, frameCount));
}