#ifndef __PIXEL_OS__HANDLERS__
#define __PIXEL_OS__HANDLERS__

#include "defines.h"
#include "lib.h"

#include "keyboard.h"
#include "display.h"

void keyboardHandler();
void syscallHandler(ddword a, ddword b, ddword c, ddword d);
void TTHandler();
#include "sound.h"
#include "timer.h"

void timerHandler();

#endif
