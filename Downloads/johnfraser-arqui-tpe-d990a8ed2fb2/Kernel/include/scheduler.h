#ifndef __PIXEL_OS__SCHEDULER__
#define __PIXEL_OS__SCHEDULER__

#include "defines.h"
#include "stddef.h"
#include "memory.h"

void initScheduler();
void addProcess(Process *p);
processNode * deleteProcess(Process *p);
void scheduling();


#endif
