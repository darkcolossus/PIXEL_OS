#ifndef __PIXEL_OS__HANDLERS__
#define __PIXEL_OS__HANDLERS__

#include "defines.h"
#include "lib.h"

#include "keyboard.h"
#include "display.h"

void keyboardHandler();
void syscallHandler(uint64_t a, ddword b, ddword c, ddword d);
uint64_t TTHandler(uint64_t stack);
#include "sound.h"
#include "timer.h"

void timerHandler();

#endif
