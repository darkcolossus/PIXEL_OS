#include "messageQueue.h"

static messageQueue * mq;

void messageQueueInit(){
	mq = kmalloc(sizeof(messageQueue));
	mq->first = NULL;
	mq->size = 0;
}

void createMessage(uint64_t emisor, uint64_t receptor, void * content, uint64_t size){
	message * newMessage = kmalloc(sizeof(message));

	newMessage->msg = content;
	newMessage->msgSize = size;
	newMessage->senderId = emisor;
	newMessage->next = NULL;

	addMessageToQueue(newMessage, receptor);
}

void addMessageToQueue(message * newMessage, uint64_t receptor){
	msgQueueNode * mqNode = getQueueNode(receptor); //looks for the specified node in the mq

	if(mqNode->msgs == NULL){
		mqNode->msgs = newMessage;
		mqNode-> lastmsg = newMessage;
		return;
	}

	mqNode->lastmsg->next = newMessage; //para no perder la lista de msgs
	mqNode->lastmsg = newMessage; //para saber cual es el ultimo msgs

}

msgQueueNode * getQueueNode(uint64_t receptor){
	msgQueueNode * currentNode = mq->first;

	if(currentNode == NULL){ //mq is empty
		currentNode = kmalloc(sizeof(msgQueueNode));
		currentNode-> id = receptor;
		currentNode-> msgs = NULL;
		currentNode-> lastmsg = NULL;
		currentNode->next = NULL;
		mq->first = currentNode;
		mq->size++;
		return currentNode; 
	}

	while(currentNode->next != NULL){     //search for node in mq
		if(currentNode->id == receptor){
			return currentNode;
		}

		currentNode = currentNode->next;
	}

	if(currentNode->id == receptor){  //asks if last mqnode is the chosen one
		return currentNode;
	}

		msgQueueNode * newNode =  kmalloc(sizeof(msgQueueNode));
		newNode-> id = receptor;
		newNode-> msgs = NULL;
		newNode-> lastmsg = NULL;
		newNode->next = NULL;
		currentNode->next = newNode; //add new node to mq
		mq->size++;
		return newNode;
}

int messageQueueSize(){
	return mq->size;
}

int existsMessage(uint64_t receptor){
	msgQueueNode * node = getQueueNode(receptor);
	return node->msgs == NULL? 0:1;
}

void deleteMessagesFromReceiver(uint64_t receptor){
	msgQueueNode * previous = mq->first;
	msgQueueNode * current = mq->first->next;

	if(previous == NULL)
		return ;

	if(previous->id == receptor){
		mq->first = mq->first->next;
		mq->size--;
		return;
	}

	while(current!=NULL){
		if(current->id == receptor){
			previous->next = current->next;
			mq->size--;
			return;
		}
		previous = current;
		current = current->next;
	}

}


message * getFirstMessage(uint64_t emisor, uint64_t receptor){
	msgQueueNode * node = getQueueNode(receptor);

	if(node == NULL)
		return NULL;

	message * msgs = node->msgs;
	message * firstMessage = NULL;
	message * queueMessage = NULL;

	while(msgs != NULL){
		if(msgs->senderId == emisor){
			if(queueMessage == NULL){
				queueMessage = kmalloc(sizeof(message));
				firstMessage = queueMessage;
				queueMessage->senderId = msgs->senderId;
				queueMessage->msg = msgs->msg;
				queueMessage->msgSize = msgs->msgSize;
				queueMessage->next = NULL;
			}else{
				message * newNode = kmalloc(sizeof(message));
				newNode->senderId = msgs->senderId;
				newNode->msg = msgs->msg;
				newNode->msgSize = msgs->msgSize;
				newNode->next = NULL;
				queueMessage->next = newNode;
				queueMessage = queueMessage->next;
			}
		}
		msgs = msgs->next;
	}
	//node->msgs = node->msgs->next;
	return firstMessage;

}


message *  readMessage(uint64_t sender, uint64_t receiver){
    msgQueueNode * node = getQueueNode(receiver);
    message * msgs= node ->msgs;
    while(msgs != NULL){
        if(msgs->senderId == sender){
            message* aux = msgs;
            msgs = msgs->next;
            return aux;
        }
        msgs = msgs->next;
    }
    addToBlocked(receiver);
    readMessage(receiver, sender);
    return NULL;
   
}