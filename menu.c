/*----------------------------------------------------------------------------
 * File Name:    Thread.c
 * Lab Number: Lab 4 Part 1
 * Purpose: Target (Demo) Version
 *----------------------------------------------------------------------------
 * Name: Timmy Huy Xuan Ngo
 * Student Number: 501031027
 *----------------------------------------------------------------------------*/
 
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "LED.h"
#include "GLCD.h"
#include <stdio.h>
#include <LPC17xx.h>
#define osObjectsPublic                     // define objects in main module
#include "osObjects.h"                      // RTOS object definitions
#include "RTE_Components.h"             // Component selection
#include <stdint.h>
#include "KBD.h"

// mp3Player.c
extern int funcMP3Player (void);

// photoGallery.c
extern int funcPhotoGallery (void);

// gameMenu.c
extern int funcGameMenu (void);

// gameTicTacToe.c
extern int funcTicTacToe (void);

// gameBlackJack.c
extern int funcBlackJack (void);

#define __USE_LCD   0
#define __FI_LARGE  1
#define __FI_SMALL 	0

//ITM Stimulus Port definitions for printf //////////////////
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

// Functions
int optionDecoder (void);
int optionManager (void);
void displayOptions (void);
int loadingScreen (unsigned int);

// GLCD
struct __FILE { int handle;  };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) {
  if (DEMCR & TRCENA) {
    while (ITM_Port32(0) == 0);
    ITM_Port8(0) = ch;
  }
  return(ch);
}

// Delay function
void Delay(uint32_t count) {
    for (uint32_t i = 0; i < count; i++);
}

void displayOptions (void) {
	
	GLCD_Clear(White);                         /* Clear graphical LCD display   */
	GLCD_SetTextColor(Black);
	GLCD_DisplayString(0, 0, __FI_SMALL, "TIME HERE");							// Time
	GLCD_DisplayString(1, 0, __FI_LARGE, "   COE718 Project:  ");			// Course Info
	GLCD_DisplayString(2, 0, __FI_LARGE, "    Media Center    ");		// Title
	GLCD_DisplayString(9, 15, __FI_SMALL, " Timmy Huy Xuan Ngo ");	// Name
	GLCD_DisplayString(10, 15, __FI_SMALL, " 501031027 ");					// Student Num
	GLCD_SetBackColor(Yellow);
	GLCD_DisplayString(4, 3, __FI_LARGE, " MP3 Player    ");		// MP3 Player
	GLCD_DisplayString(5, 3, __FI_LARGE, " Photo Gallery ");		// Photo Gallery
	GLCD_DisplayString(6, 3, __FI_LARGE, " Game Menu     ");		// Game Menu
	GLCD_SetBackColor(White);
	GLCD_DisplayString(25, 10, __FI_SMALL, " Use Joystick To Navigate ");		// Info
	GLCD_DisplayString(26, 7, __FI_SMALL, " Press Joystick to Select ");		// Info Conti.
	Delay(1000000);
}

int main (void) {
	
	// Initializations
  GLCD_Init();                               /* Initialize graphical LCD (if enabled */
	KBD_Init();
	LED_Init();
	
	// Display Options
	displayOptions();
	
	// Joystick Functionality Selection Logic
	unsigned int tempLine = 4;
	unsigned int tempJoyStickState = NULL;
	
	for (;;) {
		switch(get_button()) {
			case KBD_SELECT:
				tempJoyStickState = NULL;
				GLCD_DisplayString(tempLine, 1, __FI_LARGE, "o");
				loadingScreen(tempLine);
				displayOptions();
				break;
			case KBD_UP:
				tempJoyStickState = KBD_UP;
				if (tempJoyStickState == KBD_UP && tempLine > 4) {
					Delay(1000000);
					tempLine--;
					Delay(1000000);
				}
				break;
			case KBD_DOWN:
				tempJoyStickState = KBD_DOWN;
				if (tempJoyStickState == KBD_DOWN && tempLine < 6) {
					Delay(1000000);
					tempLine++;
					Delay(1000000);
				}
				break;
			default:
				for (int i = 4; i <= 6;) {
					if (i == tempLine) {
						Delay(100000);
						GLCD_DisplayString(tempLine, 1, __FI_LARGE, ">");
					} else {
						Delay(100000);
						GLCD_DisplayString(i, 1, __FI_LARGE, " ");
					}
					i++;
				}
		}
	}
}

int loadingScreen(unsigned int tempSelection) {
	
	GLCD_Clear(White);
	GLCD_SetBackColor(White);
	GLCD_SetTextColor(Black);
	GLCD_DisplayString(3, 6, __FI_LARGE, "Loading...");
	Delay(10000000);
	
	// Decode Functionality Selected
	int exitFlag = 0;
	
	while (exitFlag == 0) {
		switch(tempSelection) {
			case 4:
				GLCD_DisplayString(5, 3, __FI_LARGE, "MP3 Player");
			  funcMP3Player();
				Delay(100000000);
				exitFlag = 1;
				break;
			case 5:
				GLCD_DisplayString(5, 3, __FI_LARGE, "Photo Gallery");
				funcPhotoGallery();
				Delay(100000000);
				exitFlag = 1;
				break;
			case 6:
				GLCD_DisplayString(5, 3, __FI_LARGE, "Game Menu");
				funcGameMenu();
				Delay(100000000);
				exitFlag = 1;
				break;
			default:
				GLCD_DisplayString(5, 3, __FI_LARGE, "Error");
				Delay(30000000);
		}
	}
}