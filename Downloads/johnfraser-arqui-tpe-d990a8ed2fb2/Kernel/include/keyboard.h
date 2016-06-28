#ifndef __PIXEL_OS__KEYBOARD__
#define __PIXEL_OS__KEYBOARD__

#include "defines.h"

void kKBInitialize();
char kCheckSpecial(unsigned char keycode);
unsigned char keyCodeToASCII(unsigned char keycode);
void kKBKeyReceived(unsigned char keycode);
void kKBInsertKey(unsigned char asciiValue);
void kKBRemoveLastKey();
unsigned char kKBGetKey();
bool kKBIsAlpha(unsigned char keycode);
bool kKBBufferIsEmpty();

#endif
