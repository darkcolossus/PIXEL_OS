#include "memory.h"
#include "defines.h"
#include <assert.h>
#include <string.h>
#include <naiveConsole.h>
#include <stdint.h>

#define NODE_UNUSED 0
#define NODE_USED 1
#define NODE_SPLIT 2
#define NODE_FULL 3

static const uint64_t pageSize = 0x1000;

static uint64_t * stackStart = (uint64_t*)(10*0x100000);
static uint64_t * stackEnd = (uint64_t*)(12*0x100000);
static uint64_t * stackCurrent;

static void * mallocBuff = (6*0x100000);
static void * lastMalloc;

void kfree( void * m ){

}
void * kmalloc(int len){
	lastMalloc = mallocBuff;
	if((mallocBuff + len)> stackStart){
		// NO MORE MEMORTY
		ncPrint("No more mem!");
	}
	mallocBuff += len * sizeof(char);

	return lastMalloc;
}

void free(struct buddy * self){

}

void pageManagementInit(){
	uint64_t endMem = 512 * 0x100000;
	uint64_t freeMemory =  (uint64_t) stackEnd;
	stackCurrent = stackStart;
	uint64_t frames = (endMem - freeMemory) / pageSize;


	ncPrint("  Cantidad de frames: ");
	ncPrintDec(frames);
	ncNewline();

	uint64_t i;
	for(int i=0; i<frames ;i++){
		*stackCurrent = freeMemory + (i* pageSize);
		stackCurrent ++;
	}


//Testeo de paging
/*
uint64_t a = (uint64_t)pageAlloc();
ncPrint("  Direccion: 0x");
ncPrintHex(a);
ncNewline();

a = (uint64_t)pageAlloc();
ncPrint("  Direccion: 0x");
ncPrintHex(a);
ncNewline();

*/
}


void * pageAlloc(){
	if(stackCurrent-1 == stackStart ){
		//No hay mas memoria
	}
	stackCurrent--;
	memset((void *)*stackCurrent,0,4096);
	return (void *) (*stackCurrent);
}

void pageFree(void * direction){
	uint64_t actual = (uint64_t)direction;
	 if(actual % pageSize != 0){
		 *stackCurrent = (actual & 0xFFFFFFFFFFFFF000);
	 }else {
	 	*stackCurrent = (uint64_t) direction;
	}
	stackCurrent++;
}
