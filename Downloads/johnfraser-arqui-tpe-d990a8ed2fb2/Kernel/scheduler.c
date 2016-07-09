#include "scheduler.h"
#include "defines.h"
pNodeList * waitingPList;
pNodeList * bloquedPList;
process * lastP;
int isKernelStack;

void initilizeScheduler(){
  lastP= NULL;
  waitingPList = kmalloc(sizeof(struct pNodeList));
  waitingPList->current = NULL;
  bloquedPList = kmalloc(sizeof(struct pNodeList));
  bloquedPList->current = NULL;
  waitingPList->size = 0;
  bloquedPList->size =0;
  isKernelStack = 1;
}
void addProcessToWaiting(process * p){
  add(p,waitingPList);
}

void addProcessToBloqued(process * p){
  add(p,bloquedPList);
}

void add(process *p , pNodeList *list){
  if(p == NULL)
    return;
  pNode * newNode = kmalloc(sizeof(struct pNode));
  if(newNode == NULL)
    return;
  newNode->process = p;
  kDisableInterrupts();
  if(list-> current == NULL){
    list-> current = newNode;
    newNode->next = newNode;
    newNode->prev = newNode;
    return;
  }
  list->current->prev->next = newNode;
  newNode->prev = list->current->prev;
  newNode->next = list->current;
  list->current->prev = newNode;
  list->size +=1;
  kEnableInterrupts();

  return;

}

process * getLastP(){
  return lastP;
}

process * getCurrentWaiting(){
  return waitingPList->current->process;
}
process * nextProcess(){
  process *p;
  if(waitingPList->current->next->process->pid ==1 )
    waitingPList->current = waitingPList->current->next;
  p = waitingPList->current->next->process;
  lastP = waitingPList->current->process;
  waitingPList->current = waitingPList->current->next;
  return p;
}


uint64_t changeContext(uint64_t rsp){
  process *p;
  if(isKernelStack == 1){
      p = getCurrentWaiting();
      if(p==NULL)
        return 0;
      p->state = RUNNING;
      p->kernelStack = rsp;
      isKernelStack = 0;
      return p->rsp;
  }else{
      p=getCurrentWaiting();
    if(p==NULL)
      return 0;
      p->rsp = rsp;
      if(p->state != BLOCKED)
        p->state = WAITING;
      nextProcess();
      isKernelStack = 1;
      return p->kernelStack;
  }

}
void printWaiting(){
  printList(waitingPList);
}

void printList(pNodeList * pq){
   kputNewLine();

	 if(pq == NULL)
	 	return;
   pNode *ptr = pq->current;
	 if(ptr == NULL)
		return;
 	 kputString("PID\t\t\tName\t\t\tState\t\t\tMemory");
	 kputNewLine();
	 int i;
   kDisableInterrupts();
   int state;
	 for(i=0;i<pq->size+1;i++){
		 int pid = ptr->process->pid;
     kprintDec(pid);
     kputString("\t\t\t   ");
		 kputString(ptr->process->name);
		 kputString("\t\t\t");
		 state = ptr->process->state;
     if(state == 0){
        kputString("WAITING");
     }else if(state == 1){
        kputString("BLOCKED");
     }else{
        kputString("RUNNING");
     }
     kputString("\t\t\t");
     kprintHex((uint64_t)ptr->process->rsp);
		 kputNewLine();
		 ptr = ptr->next;
   }
   kEnableInterrupts();
  
}

void killCurrentProcess(){
  process * p = getCurrentWaiting();
  kDisableInterrupts();
  int ok = killProcess(p->pid,waitingPList);
  if(ok == 0)
    return;
  killProcess(p->pid,bloquedPList);
    kEnableInterrupts();
}

int killProcess(int pid, pNodeList * list){
  process * p = removeProcess(pid,list);
  if(p == NULL)
    return -1;
  pageFree(p->rsp);
  return 0;
}

process * removeProcess(int pid, pNodeList * list){
  pNode * actual = list->current;
  pNode * prev = list->current->prev;
  int count = list->size;
  if(actual == NULL)
    return NULL;
  if(actual->process->pid == pid && count ==1){
    list->current = NULL;
    list->size--;
    return actual->process;
  }
  while(actual->process->pid != pid && count > 0){
    prev =  actual;
    actual =  actual->next;
    count--;
  }

  if(actual->process->pid == pid){
    prev->next = actual->next;
    actual->next->prev = prev;
    list->size--;
    return actual->process;
  }
  return NULL;

}
int isBlockedListNull(){
  return bloquedPList->current == NULL?1:0;
}

int blockProcess(int pid){
  kDisableInterrupts();
  process *p = removeProcess(pid,bloquedPList);
  if(p ==NULL){
    kEnableInterrupts();
    return -1;
  }
  p->state= BLOCKED;
  addProcessToBloqued(p);
  kEnableInterrupts();
  return 0;
}

int unblockProcess(int pid){
  kDisableInterrupts();
  process *p = removeProcess(pid,bloquedPList);
  if(p == NULL){
    kEnableInterrupts();
    return -1;
  }
  p->state = WAITING;
  addProcessToWaiting(p);
  kEnableInterrupts();
  return 0;
}
