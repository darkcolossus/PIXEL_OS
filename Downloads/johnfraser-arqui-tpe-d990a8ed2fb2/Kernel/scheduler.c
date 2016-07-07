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
//kputString("a");
  process *p;
  if(isKernelStack == 1){
    //  kputString("Vengo del kernel!");
    //  kputString("Este es el pid: ");
      p = getCurrentWaiting();
      if(p==NULL)
        return 0;
    //  kprintDec(p->pid);
    //  kputNewLine();
      p->kernelStack = rsp;
      isKernelStack = 0;
      return p->rsp;
  }else{
  //  kputString("Vengo con el rsp del usuario");
  //  kputString("Este es el pid: ");
  //  kprintDec(p->pid);
  //  kputNewLine();
      p=getCurrentWaiting();
    if(p==NULL)
      return 0;
      p->rsp = rsp;
      nextProcess();
      isKernelStack = 1;
      return p->kernelStack;
  }

}

void killCurrentProcess(){

}
