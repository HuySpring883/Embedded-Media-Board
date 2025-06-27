#include "KBD.h"
#include "GLCD.h"

#define __FI_LARGE  1
#define __FI_SMALL 	0

// menu.c
extern int Delay(uint32_t);
extern void displayOptions(void);

int funcMusicRhythm (void) {
	
	GLCD_Clear(White);
	Delay(3000000);
	GLCD_DisplayString(0, 3, __FI_LARGE, " Music Rhythm ");
	Delay(3000000);
	
	int exitFlag = 1;
	
	while (exitFlag == 1) {
		if (get_button() == KBD_SELECT) {
			GLCD_DisplayString(4, 3, __FI_LARGE, " Exiting ");
			Delay(30000000);
			exitFlag = 0;
		}
	}
	
	displayOptions();
	return 0;
}