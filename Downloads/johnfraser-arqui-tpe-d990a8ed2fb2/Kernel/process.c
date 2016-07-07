#include "process.h"
#include "defines.h"
static int pid = 1;
static void * const shellCodeModuleAddress = (void*)0x400000;
int getPID();
void * toStackAddress(void * page);
void * fillStackFrame(EntryPoint entryPoint, void * userStack , uint64_t argc,char argv[]);
int wrapper(EntryPoint func, uint64_t argc, void *argv);
extern void printWaiting();

process *  createProcess(EntryPoint func,const char * name, uint64_t argc, char * argv[]){
	process * p = kmalloc(sizeof(struct process));

	p->name = kmalloc(kstrlen(name) + 1);
	memcpy(p->name, name, kstrlen(name) + 1);
	p->pid = getPID();
	uint64_t rsp = pageAlloc();
	rsp = toStackAddress(rsp);
	p->rsp = fillStackFrame(func,rsp,argc,argv);
	p->state = WAITING;

	return p;
}

int getPID(){
	int toReturn = pid;
	pid++;
	return toReturn;
}


void * toStackAddress(void * page){
	return page +  0x1000 - sizeof(struct stackFrame); //PageAllocator::PageSize; VER
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
	frame->rflags = 0x202;
	frame->rsp =	(uint64_t)&(frame->base);
	frame->ss = 	0x000;
	frame->base =	0x000;

	return &(frame->gs);
}

int wrapper(EntryPoint func, uint64_t argc, void *argv){
	func(argc, argv);
	killCurrentProcess();
	TTInterruptHandler() ;
	return 0;
}


static uint64_t shellProcess(){
  while(1){
	//	kDisableInterrupts();
	//	ncPrint("Este es el shell 1");
	//	kEnableInterrupts();
    //((EntryPoint)shellCodeModuleAddress)();
		//kputString("1");
	//	kputNewLine();
  }
}

static uint64_t shellProcess2(){
  while(1){
	//kputString("2");
	//kputNewLine();
	//((EntryPoint)shellCodeModuleAddress)();
  }
}
static uint64_t shellProcess3(){
  while(1){
		//kDisableInterrupts();
		//ncPrint("IDLEEEE");
		//kEnableInterrupts();
    //((EntryPoint)shellCodeModuleAddress)();
		//kputString("shellProcess3");
		//kputNewLine();
  }
}

void printWaitingProcesses(){
	process * p  = createProcess(printWaiting,"PS",0,NULL);
	addProcessToWaiting(p);
}



void initEssencialProcesses(){

	process * p  = createProcess(shellProcess3,"IDLE",0,NULL);
	addProcessToWaiting(p);
	p = createProcess(shellProcess,"Shell1",0,NULL);
	addProcessToWaiting(p);
	p = createProcess(shellProcess2,"Shell2",0,NULL);
	addProcessToWaiting(p);

}
