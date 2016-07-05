#ifndef __PIXEL_OS__MESSAGEQUEUE__
#define __PIXEL_OS__MESSAGEQUEUE__

#include "defines.h"
#include "memory.h"
#include "stddef.h"

void messageQueueInit();
void createMessage(uint64_t emisor, uint64_t receptor, void * content, uint64_t size);
void addMessageToQueue(message * newMessage, uint64_t receptor);
msgQueueNode * getQueueNode(uint64_t receptor);
int messageQueueSize();
int existsMessage(uint64_t receptor);
void deleteMessagesFromReceiver(uint64_t receptor);
message * getFirstMessage(uint64_t emisor, uint64_t receptor);

#endif