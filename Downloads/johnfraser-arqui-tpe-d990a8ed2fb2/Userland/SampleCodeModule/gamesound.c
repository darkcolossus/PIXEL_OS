#include <stdint.h>
#include "include/lib.h"
#include "include/defines.h"
#include "include/libasm.h"
#include "include/sound.h"
int gamesound(){
	while(1){
		//should check if sound has arrived from game,
		runSyscall(BEEP,0x0,0x0,0x0);
	}
}