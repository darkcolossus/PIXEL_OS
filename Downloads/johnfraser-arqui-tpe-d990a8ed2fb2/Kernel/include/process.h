#ifndef __PIXEL_OS__PROCESS__
#define __PIXEL_OS__PROCESS__


#include "memory.h"
#include "defines.h"


process* initProcess(EntryPoint entryPoint, char* name);

void * toStackAddress(void * page);
void * fillStackFrame(void * entryPoint, void * userStack);
void initializeRequiredProcesses();

#endif
