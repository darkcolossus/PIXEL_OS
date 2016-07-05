#include "process.h"
#include "defines.h"
#include <naiveConsole.h>

void * userStackPage;
void * kernelStackPage;
static void * const shellCodeModuleAddress = (void*)0x400000;

static int pid = 1;
extern wrapper;

process* initProcess(EntryPoint entryPoint, char *name,uint64_t argc,char* argv[]){

	process * newProcess = (process *) kmalloc(sizeof(process));
	//	ncPrint("  Entre a crear un proces");
	//allocate space for userStack and kernelStack
	userStackPage = (void *)pageAlloc();//pa.alloc(1); VER
	kernelStackPage = (void*)pageAlloc();//pa.alloc(1); VER
	newProcess->entryPoint = entryPoint;
	int length = kstrlen(name);
	newProcess->name = kmalloc(sizeof(char) * length +1);
	memcpy(newProcess->name, name, length);

	//page allocating assignment
	newProcess->userStack = fillStackFrame(entryPoint,toStackAddress(userStackPage),argc,argv);
	//newProcess->kernelStack = toStackAddress(kernelStackPage);
	newProcess->PID = getPID();
	newProcess->status = WAITING;

	return newProcess;
}


int getPID(){
	int toReturn = pid;
	pid++;
	return toReturn;
}
void * toStackAddress(void * page){
	return page +  0x1000 - 0x10; //PageAllocator::PageSize; VER
}



void * fillStackFrame(EntryPoint entryPoint, void * userStack , uint64_t argc,char argv[]){
	stackFrame * frame = (stackFrame*)userStack;
	frame->gs =		0x001;
	frame->fs =		0x002;
	frame->r15 =	0x003;
	frame->r14 =	0x004;
	frame->r13 =	0x005;
	frame->r12 =	0x006;
	frame->r11 =	0x007;
	frame->r10 =	0x008;
	frame->r9 =		0x009;
	frame->r8 =		0x00A;
	frame->rsi =	argc;
	frame->rdi =	(uint64_t)entryPoint;
	frame->rbp =	0x00D;
	frame->rdx =	(uint64_t)argv;
	frame->rcx =	0x00F;
	frame->rbx =	0x010;
	frame->rax =	0x011;

	frame->rip =	(uint64_t)&wrapper;
	frame->cs =		0x008;
	frame->eflags = 0x202;
	frame->rsp =	(uint64_t)&(frame->base);
	frame->ss = 	0x000;
	frame->base =	0x000;

	return &(frame->gs);
}



static uint64_t shellProcess(){
  while(1){



    ((EntryPoint)shellCodeModuleAddress)();

  }
}

void createIdleProcess(){
	process *p = initProcess(NULL, "idle",0,NULL);
	//TODO: TENEMOS QUE AGREGARLO A LA QUEUE WAITING
	addProcessToWaiting(p);
}

void initializeRequiredProcesses(){
	createIdleProcess();
	process * shellInit = initProcess(shellProcess,"Shell 1",0,NULL);
	addProcessToWaiting(shellInit);
	shellInit = initProcess(shellProcess,"Shell 2",0,NULL);
	addProcessToWaiting(shellInit);
	shellInit = initProcess(shellProcess,"Shell 3",0,NULL);
	addProcessToWaiting(shellInit);
}
