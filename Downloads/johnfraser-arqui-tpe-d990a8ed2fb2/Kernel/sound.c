#include "include/defines.h"
#include "include/sound.h"
#include "include/lib.h"

#include "include/display.h"

extern void _enable_sound();
extern void _disable_sound();

char linkState = FALSE;

void soundSet(char power, word freq) {

	if(power == TRUE) {
		

		if(freq != 0) {

			int countdown = 1193180 / freq;					// send countdown
			short int low = (short int) countdown;
			short int high = (short int) (countdown >> 8);
			kout(PORT_TIMER_PREP, 0xB6);				//prep timer to recieve new countdown
			kout(PORT_TIMER_2_DATA, low);
			kout(PORT_TIMER_2_DATA, high);
		}
		if(linkState == FALSE) {
			uint8_t tmp = kin(0x61);
			tmp = tmp | 3;
			kout(0x61, tmp);
			_enable_sound();
			linkState = TRUE;
		}
	} else if(power == FALSE) {
		uint8_t tmp = kin(0x61);
		tmp = tmp | 252;
		kout(0x61, tmp);
		_disable_sound();
		linkState = FALSE;
	}
	return;
}
