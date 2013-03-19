#ifndef MESSAGEQUEUE
#define MESSAGEQUEUE

#include <list>
using namespace std;

#include "Message.h"

class MessageQueue
{
public:
	MessageQueue();
	MessageQueue(int x, int y, int size);
	void addMessage(const char*, Color c = Color());
	void displayMessages();
	
	int x, y;
	unsigned int size;
	list<Message> messages;
};

#endif
