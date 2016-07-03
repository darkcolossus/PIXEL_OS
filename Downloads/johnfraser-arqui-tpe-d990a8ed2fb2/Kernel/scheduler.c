#include "scheduler.h"

processNode * first ;
processNode * last ;


void addProcess(process * p){
	processNode * newProcess = (processNode *) kmalloc(sizeof(processNode));
	newProcess->currentProcess = p;

	if(first == NULL){
		last = newProcess;
	}else{
		first->previous = newProcess;
	}

	//point it to old first process
   newProcess->next = first;

   //point first to new first process
   first = newProcess;
}


processNode* deleteProcess(process * p){

   //start from the first process
    processNode* actual = first;
    processNode* before = NULL;

   //if list is empty
   if(first == NULL){
      return NULL;
   }

   //navigate through list
   while(actual->currentProcess != p){
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

   return actual;
}

void scheduling(){
	first = first->next;
}


void * contextSwitching(void * rsp){
	process * newProcess = first->currentProcess;
	newProcess->userStack = rsp;
	return newProcess->kernelStack;
}
