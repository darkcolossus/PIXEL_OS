#ifndef __PIXEL_OS__PROCESS__
#define __PIXEL_OS__PROCESS__


#include "memory.h"
#include "defines.h"
#include "stddef.h"


process* initProcess(EntryPoint entryPoint, char* name);
void createIdleProcess();
void * toStackAddress(void * page);
void * fillStackFrame(void * entryPoint, void * userStack);
void initializeRequiredProcesses();

#endif
