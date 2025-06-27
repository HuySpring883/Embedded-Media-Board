#include "KBD.h"
#include "GLCD.h"
#include "audio_splashscreen.c"

#define __FI_LARGE  1
#define __FI_SMALL 	0

// menu.c
extern int Delay(uint32_t);
extern void displayOptions(void);

// usbdmain.c
extern int playAudio (void);

// Functions
int funcMP3Player (void);
int menuMP3Player (void);

int menuMP3Player (void) {
	
	GLCD_Clear(White);
	GLCD_DisplayString(0, 3, __FI_LARGE, " MP3 Player ");
	GLCD_Bitmap(50, 50, 200, 200, AUDIO_SPLASHSCREEN_PIXEL_DATA);
	GLCD_DisplayString(8, 3, __FI_LARGE, "Press Joystick");
	GLCD_DisplayString(9, 5, __FI_LARGE, "To exit");
	Delay(3000000);
	
}

int funcMP3Player (void) {
	
	menuMP3Player(); // Set up and Play audio
	playAudio();
	
	GLCD_DisplayString(4, 3, __FI_LARGE, (unsigned char *)" Exiting ");
	Delay(30000000);
	displayOptions();
	
	int exitFlag = 1;
	
	while (exitFlag == 1) {
		if (get_button() == KBD_LEFT) {
			exitFlag = 0;
			break;
		}
	}
	
	return 0;
	
}