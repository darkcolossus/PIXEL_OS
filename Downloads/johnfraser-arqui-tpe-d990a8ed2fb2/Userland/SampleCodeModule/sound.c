#include <stdint.h>
#include "include/defines.h"
#include "include/libasm.h"
#include "include/lib.h"

#define TIMER_TICK_DELAY 55 		// milliseconds

static char * const DataModule = (char*)0x500000;

static word const noteMap[6][7] = {
	//A  B   C   D   E   F   G   H   I   J   K   L
	{27, 31, 33, 37, 41, 44, 49},				//octave 1
	{55, 62, 65, 73, 82, 87, 98},				//octave 2
	//A   B    C    D    E    F    G    H    I    J    K    L
	{110, 123, 130, 147, 165, 175, 196},		//octave 3
	{220, 247, 262, 294, 330, 349, 392},		//octave 4
	{440, 494, 523, 587, 659, 698, 784},		//octave 5
	//A   B    C     D     E     F     G     H     I     J     K     L
	{880, 988, 1046, 1175, 1319, 1397, 1568},	//octave 6
};
//  H  I J  K L			(maybe....)
// A_BC_D_EF_G_			( "real" An & Bn are as octave n+1)

/*****************************
**  Asynchronous Variables  **
*****************************/

word freqs[80];
word durations[80];
int index;

int updateCounter;
word waitFor;

/*************
**  Loader  **
*************/

word NoteToFreq(char letter, char octave) {
	if(letter=='0' || octave=='0')
		return 0;
	word output = noteMap[octave-'0'-1][letter-'A'];
	return output;
}

word CodeToMillisec(char arg1, char arg2) {
	word output = ((word)arg1-'0')*100 + ((word)arg2-'0')*10;
	return output;
}

char loader(char song_id) {
	index = 0;
	waitFor = 0;
	if( song_id<'1' || song_id>'9' || song_id>DataModule[0] ) 
		return 1;
	int offset=2;
	song_id -= '0';
	if(song_id>1){										//moves variable offset past other songs
		for (char nulls = 0; nulls < song_id-1 ; ++offset){
			if(DataModule[offset]==0)
				nulls++;
		}
	}

	int i = 0;
	for(i=0; DataModule[offset+4*i] != 0 ; i++){
		freqs[i] = NoteToFreq( DataModule[offset + 4*i], DataModule[offset + 4*i+1] );
		durations[i] = CodeToMillisec( DataModule[offset + 4*i+2], DataModule[offset + 4*i+3] );
	}
	freqs[i] = 0;
	durations[i] = 0;
	return 0;
}

/********************
**  Song Commands  **
********************/

void endSong() {
	runSyscall(SET_SOUND, 0x0, 0x0, 0x0);
	runSyscall(SET_TIMER_HANDLER, 0x0, 0x0, 0x0);
	return;
}

void nextFreq() {
	if(durations[index] == 0) {
		endSong();
	} else {
		runSyscall(SET_SOUND, 0x0, 0x1, freqs[index]);
		index++;
	}
	return;
}

void update() {				// called every TIMER_TICK_DELAY milliseconds
	if(index == 0) {
		waitFor = durations[0];
		nextFreq();
		updateCounter = 0;
		return;
	}
	updateCounter++;
	if( updateCounter*TIMER_TICK_DELAY >= waitFor ) {
		waitFor = durations[index];
		nextFreq();
		updateCounter = 0;
	}
	return;
}

int startSong(char* args) {
	if( loader(args[0])!=0 ) {
		printf("Invalid song id. Max value is ");
		putChar(DataModule[0]);
		printf(".\n");
		return 0;
	}
	printf("Starting song. To stop it before it finishes use command \"stop_song\".\n");
	runSyscall(SET_TIMER_HANDLER, 0x0, update, 0x0);
	return 0;
}

int stopSong(char* args) {
	endSong();
	return 0;
}