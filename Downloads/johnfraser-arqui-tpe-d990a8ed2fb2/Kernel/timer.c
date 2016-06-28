#include "include/timer.h"

void (*handler)() = 0;

void timerSetHandler( void (*arg)() ) {
	handler = arg;
	return;
}

void timerDelegator() {
	if(handler != 0)
		handler();
	return;
}


