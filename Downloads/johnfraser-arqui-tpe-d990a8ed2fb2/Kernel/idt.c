#include "idt.h"

extern void _outb(int arg1, int arg2);
extern void picMasterMask(int i);
extern void picSlaveMask(int i);

IDTR idtr;

void kIDTInitialize(void) {

	kGetIDTR(&idtr);
	kDisableInterrupts();

	kSetHandler(0x20, (ddword)&TTInterruptHandler, idtr.base);
	kSetHandler(0x21, (ddword)&keyboardInterruptHandler, idtr.base);
	kSetHandler(0x80, (ddword)&syscallInterruptHandler, idtr.base);

	kEnableInterrupts();

	kout(0x21, 0xFC); // Set interrupt mask to only allow keyboard CCCCC 8?
	return;
}
