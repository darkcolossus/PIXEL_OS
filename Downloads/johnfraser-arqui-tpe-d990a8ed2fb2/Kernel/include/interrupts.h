#ifndef __PIXEL_OS__INTERRUPTS__
#define __PIXEL_OS__INTERRUPTS__

#include "defines.h"

extern void kEnableInterrupts();
extern void kDisableInterrupts();
extern void kSetHandler(byte index, ddword handler, ddword idtStart);
extern void kGetIDTR(IDTR* idtr);

// Handlers

extern void keyboardInterruptHandler();
extern void syscallInterruptHandler();
extern void TTInterruptHandler();

#endif 
