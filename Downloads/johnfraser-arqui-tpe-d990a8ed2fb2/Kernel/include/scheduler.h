#ifndef __PIXEL_OS__SCHEDULER__
#define __PIXEL_OS__SCHEDULER__

#include "defines.h"
#include "stddef.h"
#include "memory.h"

void initScheduler();
void addFirstProcess(processQueue * pq,process *p);
processNode* deleteProcess(processQueue *pq ,int pid);
void scheduling();


#endif
