#include "messagging.h"

void addToBlocked(int pid){

	pNode * p = kmalloc(sizeof(pNode));
	p->process->pid = pid;

	if(isBlockedListNull){
		addProcessToBloqued(p);
		return;
	}
	blockProcess(pid);

}

void unblock(int pid){

	if(isBlockedListNull){
		return ;
	}

	unblockProcess(pid);
}

