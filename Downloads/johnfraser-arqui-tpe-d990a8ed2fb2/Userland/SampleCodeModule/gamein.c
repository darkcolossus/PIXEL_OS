#include <stdint.h>
#include "include/lib.h"
#include "include/defines.h"
#include "include/libasm.h"
#include "include/sound.h"
int game_input(){
	int pid = runSyscall(pid,0x0,0x0,0x0);
	
	int gamepid = runSyscall(MQREAD,pid,0x1,0x0);

	printf("EN INPUT LOS PROCESOS DE JUEGO E INPUT SON :%d ---- %d\n",gamepid,pid);
	char input;
	while (1){
		input = getCharx();
		if(input!= 0){
			if (input=='c'||input=='v'||input=='b'||input=='n'||input=='\n'||input=='q'){
				printf("Sending %c\n",input);
				runSyscall(MQSEND,3,4,input);
				//runSyscall(MQSEND,4,3,input);
			}
		}
	}
}

char getCharx(){
  char c;
  runSyscall(READ,0x0,&c,0x0);
  return c;
}