#ifndef __PIXEL_OS__TIMER__
#define __PIXEL_OS__TIMER__

#include "defines.h"

void timerSetHandler( void (*arg)() );
void timerDelegator();

#endif
