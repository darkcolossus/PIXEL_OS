#ifndef __PIXEL_OS__PROCESS__
#define __PIXEL_OS__PROCESS__


#include "memory.h"
#include "defines.h"


process* initProcess(void * entryPoint);

void * toStackAddress(void * page);
void * fillStackFrame(void * entryPoint, void * userStack);

#endif
