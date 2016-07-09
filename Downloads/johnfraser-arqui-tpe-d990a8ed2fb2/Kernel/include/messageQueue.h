#ifndef __PIXEL_OS__MESSAGEQUEUE__
#define __PIXEL_OS__MESSAGEQUEUE__

#include "defines.h"
#include "memory.h"
#include "stddef.h"


typedef struct message{
	void * msg;
	int msgSize;
	uint64_t senderId;
	message * next;
}message;

typedef struct msgQueueNode
{
	uint64_t id;
	message * msgs;
	message * lastmsg;
	msgQueueNode * next;
}msgQueueNode;

typedef struct messageQueue
{
	messageQueueNode * first;
	int size;
}messageQueue;

void messageQueueInit();
void createMessage(uint64_t emisor, uint64_t receptor, void * content, uint64_t size);
void addMessageToQueue(message * newMessage, uint64_t receptor);
msgQueueNode * getQueueNode(uint64_t receptor);
int messageQueueSize();
int existsMessage(uint64_t receptor);
void deleteMessagesFromReceiver(uint64_t receptor);
message * getFirstMessage(uint64_t emisor, uint64_t receptor);

#endif