#include "_Process.h"

_Process::_Process(_Process* _parent, string _name, DWORD _threshold, int framenum)
{
	parent = _parent;
	name = _name;
	threshold = _threshold;
	framenumber = framenum;
	timer = GetTickCount();
}

_Process::~_Process(void)
{
}

void _Process::Output(ofstream& out, string parent, debugLevel dl, unsigned int level, bool force)
{
	if(timer > threshold)
		force = true;
	if(dl == all || force)
	{
		//full name, short name, parent's full name, ms, framenumber, topLevel
		//Processes.push(new Process("Frame 14", "Frame 14", "framescontainer", 31, true));
		//Processes.push(new Process("Frame 14 Render Draw Objects", "Draw Objects", "Frame 14 Render", 14, false));
		out << "Processes.push(new Process(\"" << (parent == ""? name:parent + " " + name) << "\", \""
			<< name << "\", \"" << (parent == ""? "framescontainer":parent) << "\", " << timer << ", " << this->framenumber << (level == 0? ", true));":", false));") << endl;

		if(children.size() > 0)
		{
			for(vector<_Process*>::iterator index = children.begin(); index != children.end(); ++index)
				(*index)->Output(out, (parent == ""? name:parent + " " + name), dl, level + 1, force);
		}
	}
}

bool _Process::BreaksThreshold()
{
	bool breaks = false;
	if(timer > threshold)
		breaks = true;
	else
	{
		for(vector<_Process*>::iterator index = children.begin(); !breaks && index != children.end(); ++index)
		{
			if((*index)->BreaksThreshold())
			{
				breaks = true;
			}
		}
	}
	return breaks;
}