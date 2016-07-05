#ifndef __PIXEL_OS__SCHEDULER__
#define __PIXEL_OS__SCHEDULER__

#include "defines.h"
#include "stddef.h"
#include "memory.h"

void initScheduler();
void addFirstProcess(processQueue * pq,process *p);
void addLastProcess(processQueue *pq,process * p);
void addProcessToWaiting(process * p);
void addProcessToBlocked(process *p);
int deleteProcessFromWaiting(int pid);
int deleteProcessFromBloqued(int pid);
process * getProcess(processQueue *pq, 	int pid);
process * getProcessFromAll(int pid);
void blockProcess(int pid);
processNode* removeProcess(processQueue *pq ,int pid);
process * removeProcessFromWaiting(uint64_t pid);
process * removeProcessFromBloqued(uint64_t pid);
void printAll();
void deleteProcessWrapper(int pid);
int deleteProcess(processQueue *pq ,int pid);
process * scheduling();
void* userToKernel(void * rsp);
void* kernelToUser();




#endif
