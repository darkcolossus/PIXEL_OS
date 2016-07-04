#include "scheduler.h"
#include <naiveConsole.h>
//processNode * first = NULL;
//processNode * last =NULL ;
static processQueue * waitingProcesses;
static processQueue * bloquedProcesses;

void initProcessQueues(){
	waitingProcesses = kmalloc(sizeof(struct processQueue));
	waitingProcesses->size =0;
	waitingProcesses->first = NULL;
	waitingProcesses->last =NULL;

	bloquedProcesses = kmalloc(sizeof(struct processQueue));
	bloquedProcesses->size = 0;
	bloquedProcesses->first=NULL;
	bloquedProcesses->last =NULL;
}

void addFirstProcess(processQueue * pq ,process * p){

	processNode * newProcess = (processNode *) kmalloc(sizeof(processNode));
	newProcess->currentProcess = p;
	processNode * first = pq->first;
	processNode * last = pq->last;

	if(first == NULL){
		last = newProcess;
	}else{
		first->previous = newProcess;
	}

	//point it to old first process
   newProcess->next = first;

   //point first to new first process
   first = newProcess;

	pq->first = first;
	pq->last = last;
}


processNode* deleteProcess(processQueue *pq ,int pid){
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
   pageFree(actual->currentProcess->userStack);
   pageFree(actual->currentProcess->kernelStack);
   kfree(actual);
   return actual;
}
/*
void scheduling(){
	first = first->next;
}
*/

void printAll(processQueue *pq){
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
/*
void * contextSwitching(void * rsp){
	process * newProcess = first->currentProcess;
	newProcess->userStack = rsp;
	return newProcess->kernelStack;
}
*/
