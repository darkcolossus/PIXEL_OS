#include "process.h"

void * userStackPage;
void * kernelStackPage;


process* initProcess(void * entryPoint){
	process * newProcess = (process *) kmalloc(sizeof(process));

	//allocate space for userStack and kernelStack
	userStackPage = pageAlloc();//pa.alloc(1); VER
	kernelStackPage = pageAlloc();//pa.alloc(1); VER
	newProcess->entryPoint = entryPoint;

	//page allocating assignment
	newProcess->userStack = toStackAddress(userStackPage);
	newProcess->kernelStack = toStackAddress(kernelStackPage);
	return newProcess;
}


void * toStackAddress(void * page){
	return (uint8_t *)page +  0x1000 - 0x10; //PageAllocator::PageSize; VER
}



void * fillStackFrame(void * entryPoint, void * userStack){
	stackFrame * frame = (stackFrame*)userStack - 1;
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
	frame->rsi =	0x00B;
	frame->rdi =	0x00C;
	frame->rbp =	0x00D;
	frame->rdx =	0x00E;
	frame->rcx =	0x00F;
	frame->rbx =	0x010;
	frame->rax =	0x011;
	frame->rip =	(uint64_t)entryPoint;
	frame->cs =		0x008;
	frame->eflags = 0x202;
	frame->rsp =	(uint64_t)&(frame->base);
	frame->ss = 	0x000;
	frame->base =	0x000;

	return frame;
}
