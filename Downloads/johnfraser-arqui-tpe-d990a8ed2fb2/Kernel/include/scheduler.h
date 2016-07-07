#ifndef __PIXEL_OS__SCHEDULER__
#define __PIXEL_OS__SCHEDULER__

#include "defines.h"
#include "process.h"
#include <stdlib.h>

typedef struct pNode{
  process * process;
  struct pNode * next;
  struct pNode * prev;
}pNode;

typedef struct pNodeList{
  struct pNode * current;
  uint64_t size;
}pNodeList;

void initilizeScheduler();
void addProcessToWaiting(process * p);
void addProcessToBloqued(process * p);
process * getLastP();
process * getCurrentWaiting();
process * nextProcess();
uint64_t changeContext(uint64_t rsp);
void printWaiting();
void printList(pNodeList * pq);
void killCurrentProcess();
int isBlockedListNull();
int blockProcess(int pid);
int unblockProcess(int pid);

#endif
