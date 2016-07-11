#include <stdint.h>
#include "include/lib.h"
#include "include/defines.h"
#include "include/libasm.h"
#include "include/sound.h"
int gamesound(){
	
	//startSong("2");
	int a;
	while (1){
		a= runSyscall(MQREAD,5,5,0x0);
		if(a==1){
			runSyscall(BEEP,0x0,0x0,0x0);
		}
	}
	
}