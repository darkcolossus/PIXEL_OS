#ifndef __PIXEL_OS__PROCESS__
#define __PIXEL_OS__PROCESS__


#include "memory.h"
#include "defines.h"
#include "stddef.h"


process* initProcess(EntryPoint entryPoint, char* name,uint64_t argc,char* argv[]);
void createIdleProcess();
void * toStackAddress(void * page);
void * fillStackFrame(EntryPoint entryPoint, void * userStack , uint64_t argc,char argv[]);
void initializeRequiredProcesses();

#endif
