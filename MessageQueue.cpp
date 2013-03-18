#include "MessageQueue.h"
#include "Global.h"

MessageQueue::MessageQueue()
{
	size = 10;
	x = 10;
	y = 10;
}

MessageQueue::MessageQueue(int _x, int _y, int _s)
{
	size = _s;
	x = _x;
	y = _y;
}

void MessageQueue::addMessage(const char* m, Color c)
{
	Message mess(m, c);
	messages.insert(messages.end(), mess);
	if(messages.size() > size)
		messages.erase(messages.begin());
}

void MessageQueue::displayMessages()
{
	unsigned int i = 0;
	for(list<Message>::iterator index = messages.begin(); index != messages.end(); ++i, ++index)
		drawText((*index).m, x, y + (15 * i), int((*index).c.red*255), int((*index).c.green*255), int((*index).c.blue*255));
}
