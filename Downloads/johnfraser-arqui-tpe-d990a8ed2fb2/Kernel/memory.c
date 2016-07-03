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
struct buddy * b ;


/*
void memInit(){
	b = buddyMemNew(22);

	ncPrint("  Direccion: 0x");
	uint64_t a = (uint64_t)kmalloc(4);
	ncPrintHex(a);
	ncNewline();

	ncPrint("  Direccion: 0x");
	uint64_t b = (uint64_t)kmalloc(20);
	ncPrintHex(b);
	ncNewline();

	ncPrint("  Direccion: 0x");
	uint64_t c = (uint64_t)kmalloc(3);
	ncPrintHex(c);
	ncNewline();

	ncPrint("  Direccion: 0x");
	uint64_t d = (uint64_t)kmalloc(50);
	ncPrintHex(d);
	ncNewline();

	kfree(a);
	kfree(b);
	kfree(c);
	kfree(d);

	ncPrint("  Direccion: 0x");
	uint64_t e = (uint64_t)kmalloc(32);
	ncPrintHex(e);
	ncNewline();

	ncPrint("  Direccion: 0x");
	uint64_t f = (uint64_t)kmalloc(80);
	ncPrintHex(f);
	ncNewline();

	ncPrint("Hola estoy aca");
	
}
*/
/*
void * kmalloc(int len){
	if(len <=0){
		ncPrint("  Nico se la re come doblada");
	}
	int offset = memAlloc(b,len);
	//ncPrintHex((uint64_t)offset);
	//ncNewline();
	if(offset != -1 ){
		return offset + (6*0x100000);
	}else{
		ncPrint("  ERROR!! offset!");
	}
}
*/
void kfree( void * m ){
	int offset=	((uint64_t)m )-(6*0x100000);
	memFree(b,offset);
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
/*
struct buddy * buddyMemNew(int level) {
	int size = 1 << level;
	struct buddy * self = malloc(sizeof(struct buddy) + sizeof(uint8_t) * (size * 2 - 2));
	self->level = level;
	memset(self->tree , NODE_UNUSED , size*2-1);
	return self;
}

*/
void buddyMemDelete(struct buddy * self) {

}

static inline int
is_pow_of_2(uint32_t x) {
	return !(x & (x-1));
}

static inline uint32_t
next_pow_of_2(uint32_t x) {
	if ( is_pow_of_2(x) )
		return x;
	x |= x>>1;
	x |= x>>2;
	x |= x>>4;
	x |= x>>8;
	x |= x>>16;
	return x+1;
}

static inline int
_index_offset(int index, int level, int max_level) {
	return ((index + 1) - (1 << level)) << (max_level - level);
}

static void
_mark_parent(struct buddy * self, int index) {
	for (;;) {
		int buddy = index - 1 + (index & 1) * 2;
		if (buddy > 0 && (self->tree[buddy] == NODE_USED ||	self->tree[buddy] == NODE_FULL)) {
			index = (index + 1) / 2 - 1;
			self->tree[index] = NODE_FULL;
		} else {
			return;
		}
	}
}

int
memAlloc(struct buddy * self , int s) {
	int size;
	if (s==0) {
		size = 1;
	} else {
		size = (int)next_pow_of_2(s);
	}
	int length = 1 << self->level;

	if (size > length)
		return -1;

	int index = 0;
	int level = 0;

	while (index >= 0) {
		if (size == length) {
			if (self->tree[index] == NODE_UNUSED) {
				self->tree[index] = NODE_USED;
				_mark_parent(self, index);
				return _index_offset(index, level, self->level);
			}
		} else {
			// size < length
			switch (self->tree[index]) {
			case NODE_USED:
			case NODE_FULL:
				break;
			case NODE_UNUSED:
				// split first
				self->tree[index] = NODE_SPLIT;
				self->tree[index*2+1] = NODE_UNUSED;
				self->tree[index*2+2] = NODE_UNUSED;
			default:
				index = index * 2 + 1;
				length /= 2;
				level++;
				continue;
			}
		}
		if (index & 1) {
			++index;
			continue;
		}
		for (;;) {
			level--;
			length *= 2;
			index = (index+1)/2 -1;
			if (index < 0)
				return -1;
			if (index & 1) {
				++index;
				break;
			}
		}
	}

	return -1;
}

static void
_combine(struct buddy * self, int index) {
	for (;;) {
		int buddy = index - 1 + (index & 1) * 2;
		if (buddy < 0 || self->tree[buddy] != NODE_UNUSED) {
			self->tree[index] = NODE_UNUSED;
			while (((index = (index + 1) / 2 - 1) >= 0) &&  self->tree[index] == NODE_FULL){
				self->tree[index] = NODE_SPLIT;
			}
			return;
		}
		index = (index + 1) / 2 - 1;
	}
}

void
memFree(struct buddy * self, int offset) {
//	assert( offset < (1<< self->level));
	int left = 0;
	int length = 1 << self->level;
	int index = 0;

	for (;;) {
		switch (self->tree[index]) {
		case NODE_USED:
		//	assert(offset == left);
			_combine(self, index);
			return;
		case NODE_UNUSED:
			//assert(0);
			return;
		default:
			length /= 2;
			if (offset < left + length) {
				index = index * 2 + 1;
			} else {
				left += length;
				index = index * 2 + 2;
			}
			break;
		}
	}
}

int
memSize(struct buddy * self, int offset) {
//	assert( offset < (1<< self->level));
	int left = 0;
	int length = 1 << self->level;
	int index = 0;

	for (;;) {
		switch (self->tree[index]) {
		case NODE_USED:
		//	assert(offset == left);
			return length;
		case NODE_UNUSED:
			//assert(0);
			return length;
		default:
			length /= 2;
			if (offset < left + length) {
				index = index * 2 + 1;
			} else {
				left += length;
				index = index * 2 + 2;
			}
			break;
		}
	}
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
