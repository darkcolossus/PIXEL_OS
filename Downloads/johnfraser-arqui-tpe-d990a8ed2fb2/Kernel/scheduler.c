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
void printWaiting(){
  printList(waitingPList);
}

void printList(pNodeList * pq){
		//kputString("estoy en printall");
   //start from the beginning
   kputNewLine();
	kDisableInterrupts();
	 if(pq == NULL)
	 	return;
  pNode *ptr = pq->current;
		if(ptr == NULL)
			return;
   //navigate till the end of the list
 	kputString("PID\t\t\tName\t\t\tState\t\t\tForeground\t\t\tMemory");
	kputNewLine();

   //while(ptr != NULL){
	 int i;
	 for(i=0;i<pq->size;i++){
   		 //kputString("EL NOMBRE DEL PROCESO ES:");
   		 //kputString(ptr->currentProcess->name);
   		 //kputNewLine();
		 int pid = ptr->process->pid;
		 char * str[100];
		 //kitoa(pid,str);
       kprintDec(pid);
       kputString("\t\t");
		 	kputString(ptr->process->name);
		 //kputString(str);
       kputString("\t\t");
				kprintDec(ptr->process->state);
       //kputString("RUNNING");
       //kputString("RUNNING"); ACA VA EL ESTADO
       kputString("\t\t");
       //kputString("SHELL");
       kputString("\t\t\t\t");
       kprintHex((uint64_t)ptr->process->rsp);
		 /*
		 ncPrint("Elemento: ");
		 ncPrintDec(ptr->currentProcess->PID);
		 ncNewline();
		 */
		 kputNewLine();
			ptr = ptr->next;
   }

   kEnableInterrupts();
   while(1);
}

void killCurrentProcess(){

}
