#include "keyboard.h"

kKBStatus keyboardStatus;
kKBBuffer keyboardBuffer;
unsigned char keyboardMap[2][TOTAL_KEYS] = {
	{
		0, NOT_PRINTABLE,'1','2','3','4','5','6','7','8','9','0','\'','<','\b',
		'\t','q','w','e','r','t','y','u','i','o','p','+','*','\n',
		NOT_PRINTABLE,'a','s','d','f','g','h','j','k','l',NOT_PRINTABLE,'{','}',
		NOT_PRINTABLE, '\\', 'z','x','c','v','b','n','m',',','.','-',NOT_PRINTABLE,NOT_PRINTABLE,
		NOT_PRINTABLE,SPACEBAR,NOT_PRINTABLE
	},
	{
		0, NOT_PRINTABLE,'!','@','#','$','%','&','/','(',')','=','?','>','\b',
		'\t','Q','W','E','R','T','Y','U','I','O','P','[',']','\n',
		NOT_PRINTABLE,'A','S','D','F','G','H','J','K','L',NOT_PRINTABLE,'{','}',
		NOT_PRINTABLE, '|', 'Z','X','C','V','B','N','M',';',':','_',NOT_PRINTABLE,NOT_PRINTABLE,
		NOT_PRINTABLE,SPACEBAR,NOT_PRINTABLE
	}
};

void kKBInitialize(){
	int i;
	for (i = 0; i<KEYBOARD_BUFFER_SIZE; i++) {
		keyboardBuffer.buffer[i] = EMPTY;
	}
}

bool kKBBufferIsEmpty(){
	return (keyboardBuffer.buffer[keyboardBuffer.readIndex] == EMPTY);
}

bool kKBIsAlpha(unsigned char keycode){
	return  ((keycode >= 0x10 && keycode <= 0x19) || (keycode >= 0x1E && keycode <= 0x26) || (keycode >= 0x2C && keycode <= 0x32));
}

void kKBInsertKey(unsigned char asciiValue){
	keyboardBuffer.buffer[keyboardBuffer.writeIndex++] = asciiValue;
	if(keyboardBuffer.writeIndex == KEYBOARD_BUFFER_SIZE) {
		keyboardBuffer.writeIndex = 0;
	}
}

unsigned char kKBGetKey(){

	unsigned char key = keyboardBuffer.buffer[keyboardBuffer.readIndex];
	keyboardBuffer.buffer[keyboardBuffer.readIndex++] = EMPTY;

	if (keyboardBuffer.readIndex == KEYBOARD_BUFFER_SIZE) {
		keyboardBuffer.readIndex = 0;
	}
	return key;
}

char kCheckSpecial(unsigned char keycode) {
	if(keycode == CAPS_LOCK)
		keyboardStatus.capsLockEnabled = !keyboardStatus.capsLockEnabled;
	else if (keycode == LS_PRESSED || keycode == RS_PRESSED)
		keyboardStatus.shiftEnabled = TRUE;
	else if (keycode == RS_RELEASED || keycode == LS_RELEASED)
		keyboardStatus.shiftEnabled = FALSE;
	else if(!(keycode & KEY_RELEASED)) {
		return 1;
	}
	return 0;
}

unsigned char keyCodeToASCII(unsigned char keycode) {
	bool isAlpha = kKBIsAlpha(keycode);
	bool alternate = (isAlpha && keyboardStatus.capsLockEnabled);

	if(keyboardStatus.shiftEnabled) {
		alternate = !alternate;
	}
	unsigned char asciiValue = keyboardMap[alternate][keycode];
	return asciiValue;
}

void kKBKeyReceived(unsigned char asciiValue){
	if(!(asciiValue == NOT_PRINTABLE)){
		kKBInsertKey(asciiValue);
		
	}

}
