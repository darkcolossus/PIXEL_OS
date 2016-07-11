#include "interruptsHandler.h"
#include "scheduler.h"
#include "lib.h"
#include "process.h"
#include "stddef.h"
#include "messageQueue.h"
#include "interrupts.h"
extern void _beep();
int checktest();
void printWaitingProcesses();

//int character = 0x0;

void keyboardHandler(void) {
	unsigned char status = kin(KEYBOARD_STATUS_PORT);
	unsigned char keycode = kin(KEYBOARD_DATA_PORT);

	if (status &  0x01) {

		if( kCheckSpecial(keycode) ){
			kKBKeyReceived(keyCodeToASCII(keycode));
			kputChar(keyCodeToASCII(keycode));

		}
	}
	return;
}

uint64_t syscallHandler(uint64_t a, uint64_t b, uint64_t c, uint64_t d){
	switch (a) {
		case WRITE: {
			kWrite((char*)c, (int) d);
			ksetBackspaceLimit();
			break;
		}
		case READ: {
			kRead((char*)c);
			break;
		}
		case CLEAR_SCREEN: {
			kclearScreen();
			break;
		}
		case SET_SOUND: {
			soundSet((char) c, (word) d);	//power, freq
			break;
		}
		case SET_TIMER_HANDLER: {
			timerSetHandler((void*)c);
			break;
		}
		case BEEP: {
			_beep();
			break;
		}
		case CREATE_PROCESS:{
			kDisableInterrupts();
			//addProcess(initProcess(0x400000, "prueba"));
			process * p = createProcess( b, (char *) c, 0, NULL);
			addProcessToWaiting(p);
			kprintDec(p->pid);
			kEnableInterrupts();
			return p->pid;
			break;
		}
		case DELETE_PROCESS:{
			//deleteProcessWrapper((int)d);
			break;
		}
		case LIST_PROCESSES:{
			printWaitingProcesses();
			
			
			break;
		}
		case VIDEO:{
			_video();
		}
		case PIXEL:{
			pixel_wrap((int) b, (int) c, (int) d);
			break;
		}
		case PID:{
			return getCurrentWaiting()->pid;
		}
		case MQSEND:{
			
			kDisableInterrupts();
			createMessage( b,  c, d, sizeof(uint64_t));
			kEnableInterrupts();
			break;
		}
		case MQREAD:{
			kDisableInterrupts();
			message * ms = getFirstMessage(b,c);
			
			if (ms == NULL){
				
				kEnableInterrupts();
				return 0;
			}
			
			kEnableInterrupts();
			return ms->msg;//ms->msg;
			break;
		}
		case DRAW_RECT:{
			rect_wrap(b,c,d);
			break;
		}
		case DRAW_CIRC:{
			circle_wrap(b,c,d);
			break;
		}
	}
	return 0;
}

int checktest(){
	while(1)
	{
		_beep();
	}
	return 1;
}

uint64_t TTHandler(uint64_t stack){
	timerDelegator();
	return changeContext(stack);
}
