#include "scheduler.h"
#include <naiveConsole.h>
//processNode * first = NULL;
//processNode * last =NULL ;
static processQueue * waitingProcesses;
static processQueue * blockedProcesses;
process * last;

void initProcessQueues(){
	waitingProcesses = kmalloc(sizeof(struct processQueue));
	waitingProcesses->size =0;
	waitingProcesses->first = NULL;
	waitingProcesses->last =NULL;

	blockedProcesses = kmalloc(sizeof(struct processQueue));
	blockedProcesses->size = 0;
	blockedProcesses->first=NULL;
	blockedProcesses->last =NULL;
}

void addFirstProcess(processQueue * pq ,process * p){

	processNode * newProcess = (processNode *) kmalloc(sizeof(processNode));
	newProcess->currentProcess = p;
	processNode * first = pq->first;
	processNode * last = pq->last;

	if(first == NULL){
		last = newProcess;
		first = newProcess;
		pq->currentNode =  newProcess;
	}else{
		first->previous = newProcess;
	}

	//point it to old first process
   newProcess->next = first;
	 newProcess->previous = last;
   //point first to new first process
   first = newProcess;

	pq->first = first;
	pq->last = last;
}

//No usemos esta
void addLastProcess(processQueue *pq,process * p){
   //create a link
   struct processNode *link = (struct processNode*) kmalloc(sizeof(struct processNode));
   link->currentProcess = p;
   processNode * first = pq->first;
   processNode * last = pq->last;

   if(first == NULL){
      //make it the last link
      last = link;
   }else {
      //make link a new last link
      last->next = link;
      //mark old last node as prev of new link
      link->previous = last;
   }

   //point last to new last node
   last = link;
   pq->first = first;
   pq-> last = last;
}

void addProcessToWaiting(process * p){
	addLastProcess(waitingProcesses, p);
}

void addProcessToBlocked(process *p){
	addLastProcess(blockedProcesses, p);
}

int deleteProcessFromWaiting(int pid){
	return deleteProcess(waitingProcesses, pid);
}


int deleteProcessFromBloqued(int pid){
	return deleteProcess(blockedProcesses, pid);
}

int deleteProcess(processQueue *pq,int pid){
	process * p = removeProcess(pq,pid);
	if(p == NULL)
		return -1;

	pageFree(p->userStack);
    pageFree(p->kernelStack);
    kfree(p);

    return 0;

}

void deleteProcessWrapper(int pid){

	int num = deleteProcessFromWaiting(pid);
	if (num == -1)
		deleteProcessFromBloqued(pid);
}

void createIdleProcess(){
	process *p = initProcess(NULL, "idle");
	//TODO: TENEMOS QUE AGREGARLO A LA QUEUE WAITING
}

process * getProcess(processQueue *pq, 	int pid){
	processNode * current;
	processNode * previous;

	current = pq->first;

	if(current == NULL)
		return NULL;

	if(current->currentProcess->PID == pid){
		return current->currentProcess;
	}

	while(current != NULL && current->currentProcess->PID != pid){
		current = current->next;
	}

	if(current != NULL)
		return current->currentProcess;

	return NULL;
}

process * getProcessFromAll(int pid){
	process * p = getProcess(waitingProcesses,pid);
	if(p != NULL)
		return p;

	p = getProcess(blockedProcesses, pid);

	return p;
}

process * getProcessFromWaiting(){
	return waitingProcesses->first->currentProcess;
}

void blockProcess(int pid){
	kDisableInterrupts();
	removeProcessFromWaiting(pid);

}

processNode* removeProcess(processQueue *pq ,int pid){
   //start from the first process
	processNode * first = pq->first;
	processNode * last = pq->last;
    processNode* actual = first;
    processNode* before = NULL;

   //if list is empty
   if(first == NULL){
      return NULL;
   }

   //navigate through list
   while(actual->currentProcess->PID != pid){
      //if it is last node

      if(actual->next == NULL){
         return NULL;
      }else {
         //store reference to actual process
         before = actual;

         //move to next process
         actual = actual->next;
      }
   }

   //found a match, update the process
   if(actual == first) {
      //change first to point to next process
      first = first->next;
   }else {
      //bypass the current process
      actual->previous->next = actual->next;
   }

   if(actual == last){
      //change last to point to prev process
      last = actual->previous;
   }else {
      actual->next->previous = actual->previous;
   }
	 pq->first = first;
 	 pq->last = last;

   return actual;
}

process * removeProcessFromWaiting(uint64_t pid){
	return removeProcess(waitingProcesses, pid);
}

process * removeProcessFromBloqued(uint64_t pid){
	return removeProcess(blockedProcesses, pid);
}

process * scheduling(){
	process * p;

	if(waitingProcesses->first !=NULL){
		if(isIdle(waitingProcesses->first->next)){
			waitingProcesses->first = waitingProcesses->first->next;
		}

		p = waitingProcesses->first->next->currentProcess;
		waitingProcesses->first = waitingProcesses->first->next;
		last = p;

		return p;
	}
	return p;

}

int isIdle(process *p){
	return p->PID == 1;
}

void printAll(processQueue * pq){
   //start from the beginning

    processNode *ptr = pq->first;

   //navigate till the end of the list
 	kputString("PID\t\t\tName\t\t\tState\t\t\tForeground\t\t\tMemory");
	kputNewLine();
	int i=0;
   while(i < 2){
   		 kputString("EL NOMBRE DEL PROCESO ES:");
   		 kputString(ptr->currentProcess->name);
   		 kputNewLine();
		 int pid = ptr->currentProcess->PID;
		 char * str[100];
		 //kitoa(pid,str);
       kprintDec(pid);
       kputString("\t\t");
		 kputString(ptr->currentProcess->name);
		 //kputString(str);
       kputString("\t\t");
       kputString("RUNNING");
       //kputString("RUNNING"); ACA VA EL ESTADO
       kputString("\t\t");
       kputString("SHELL");
       kputString("\t\t\t\t");
       kprintHex((uint64_t)ptr->currentProcess->userStack);
		 /*
		 ncPrint("Elemento: ");
		 ncPrintDec(ptr->currentProcess->PID);
		 ncNewline();
		 */
		 kputNewLine();
			ptr = ptr->next;
   }


}


void * contextSwitching(void * rsp){
	process * p = getProcessFromWaiting();

	if(p == NULL)
		return 0;

	//TODO: AGUANTE BOCA
	p->userStack = rsp;


	if(p->status != BLOCKED){
		p->status = WAITING;
	}

	p = scheduling();

	if(p == NULL)
		return 0;

	p->status = RUNNING;

	return p->kernelStack;
}
