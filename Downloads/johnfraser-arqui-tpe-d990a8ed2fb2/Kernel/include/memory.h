#ifndef __PIXEL_OS__MEMORY__
#define __PIXEL_OS__MEMORY__

#include "defines.h"

void memInit();
void pageManagementInit();
void * kmalloc(int len);
void kfree( void * m );
struct buddy * buddyMemNew(int level);
void buddyMemDelete(struct buddy * self);
int memAlloc(struct buddy * self , int s);
void memFree(struct buddy * self, int offset);
int memSize(struct buddy * self, int offset) ;
void * pageAlloc();

#endif
