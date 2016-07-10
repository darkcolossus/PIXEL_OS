#include "interruptsHandler.h"
#include "scheduler.h"
#include "process.h"
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

void syscallHandler(uint64_t a, ddword b, ddword c, ddword d){
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
			//addProcess(initProcess(0x400000, "prueba"));
			process * p = createProcess( &checktest, "IDLE", 0, NULL);
			addProcessToWaiting(p);
			
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
	}
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
