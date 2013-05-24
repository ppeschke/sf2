#include "_Process.h"

_Process::_Process(_Process* _parent, string _name, DWORD _threshold)
{
	parent = _parent;
	name = _name;
	threshold = _threshold;
	timer = GetTickCount();
}

_Process::~_Process(void)
{
}

void _Process::Output(ofstream& out, debugLevel dl, unsigned int level, bool force)
{
	if(timer > threshold)
		force = true;
	if(dl == all || force)
	{
		for(unsigned int i = 0; i < level; ++i)
			out << ",";
		out << name << endl;
		for(unsigned int i = 0; i < level; ++i)
			out << ",";
		out << timer << "ms" << endl;
		if(children.size() > 0)
		{
			for(vector<_Process*>::iterator index = children.begin(); index != children.end(); ++index)
				(*index)->Output(out, dl, level + 1, force);
		}
		out << endl;
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