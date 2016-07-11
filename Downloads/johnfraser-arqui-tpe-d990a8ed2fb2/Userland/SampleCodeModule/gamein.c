#include "include/lib.h"


int game_input(){
	int pid = runSyscall(pid,0x0,0x0,0x0);
	int gamepid = runSyscall(MQREAD,pid,0x1,0x0);
	char input;
	while (1){
		input = getCharx();
		if(input!= 0){
			if (input=='c'||input=='v'||input=='b'||input=='n'||input=='\n'){
				printf("Sending %c\n",input);
				runSyscall(MQSEND,pid,gamepid,input);
				runSyscall(MQSEND,gamepid,pid,input);
			}
		}
	}
}

char getCharx(){
  char c;
  runSyscall(READ,0x0,&c,0x0);
  return c;
}