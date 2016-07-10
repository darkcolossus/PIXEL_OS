#include "include/lib.h"

char getCharx();
int game_input();

int game_input(){
	char input;
	while (1){
		input = getCharx();
		if (input=='c'||input=='v'||input=='b'||input=='n'||input=='\n'){
			//Send via messagequeue
			printf("should be sneding to game\n");
		}
	}
}

char getCharx(){
  char c;
  runSyscall(READ,0x0,&c,0x0);
  return c;
}