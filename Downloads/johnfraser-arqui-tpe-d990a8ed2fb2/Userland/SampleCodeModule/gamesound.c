#include <stdint.h>
#include "include/lib.h"
#include "include/defines.h"
#include "include/libasm.h"
#include "include/sound.h"
int gamesound(){
	
	unsigned long a=0;
	startSong("2");
	while (1){
		if(a<34222222){
			a++;
		}else{
			a=0;
			startSong("2");
		}
	}
	
}