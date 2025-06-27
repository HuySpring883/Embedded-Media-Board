/*----------------------------------------------------------------------------
 * File Name:    Thread.c
 * Lab Number: Lab 4 Part 1
 * Purpose: Target (Demo) Version
 *----------------------------------------------------------------------------
 * Name: Timmy Huy Xuan Ngo
 * Student Number: 501031027
 *----------------------------------------------------------------------------*/
 
#include <stdio.h>
#include <LPC17xx.h>
#include "GLCD.h"
#include "KBD.h"

// Photos
#include "Astronaut in Space.c"
#include "Basketball.c"
#include "MCB1700 Development Board.c"

#define __FI_LARGE  1
#define __FI_SMALL 	0

// menu.c
extern int Delay(uint32_t);
extern void displayOptions(void);

// Functions
void displayPhoto (unsigned int);
int photoSelector (void);

const char *files[] = {"Astronaut, Basketball, MCB1700 Board"};

void displayPhoto (unsigned int photoNum) {
	
	int exitFlag = 1;
	
	while (exitFlag == 1) {
		switch (photoNum) {
			case 4:
				GLCD_Bitmap(0, 0, 240, 240, ASTRONAUTIMAGE_PIXEL_DATA);
				break;
			case 5:
				GLCD_Bitmap(0, 0, 240, 240, BASKETBALLIMAGE_PIXEL_DATA);
				break;
			case 6:
				GLCD_Bitmap(0, 0, 225, 204, PHOTO1_PIXEL_DATA);
				break;
			default:
				GLCD_DisplayString(4, 2, __FI_LARGE, "Photo Error");
		}
		
		if (get_button() == KBD_RIGHT)
			exitFlag = 0;
	}
}

int photoSelector (void) {
	// Joystick Functionality Selection Logic
	unsigned int tempLine = 4;
	unsigned int tempJoyStickState = NULL;
	
	int exitFlag = 1;
	
	while (exitFlag == 1) {
		Delay(1000000);
		switch(get_button()) {
			case KBD_SELECT:
				tempJoyStickState = KBD_SELECT;
				GLCD_DisplayString(tempLine, 0, __FI_LARGE, "o");
				return tempLine;
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
			case KBD_RIGHT:
				exitFlag = 0;
				GLCD_DisplayString(1, 6, __FI_LARGE, "<");
				GLCD_DisplayString(tempLine, 0, __FI_LARGE, " ");
				Delay(30000000);
				GLCD_Clear(White);
				Delay(3000000);
				GLCD_DisplayString(4, 5, __FI_LARGE, "Exiting");
				Delay(3000000);
				return 0;
			default:
				for (int i = 4; i <= 6;) {
					if (i == tempLine) {
						Delay(100000);
						GLCD_DisplayString(tempLine, 0, __FI_LARGE, ">");
					} else {
						Delay(100000);
						GLCD_DisplayString(i, 0, __FI_LARGE, " ");
					}
					i++;
				}
		}
	}
	return 0;
}

int menuPhotoGallery (void) {
	GLCD_Clear(White);
	Delay(3000000);
	GLCD_DisplayString(0, 3, __FI_LARGE, " Photo Gallery ");
	Delay(3000000);
	GLCD_SetBackColor(Red);
	GLCD_DisplayString(1, 0, __FI_LARGE, "EXIT");
	Delay(3000000);
	GLCD_SetBackColor(White);
	
	// List of Available Photos
	GLCD_DisplayString(4, 3, __FI_LARGE, "Astronaut");
	Delay(3000000);
	GLCD_DisplayString(5, 3, __FI_LARGE, "Basketball");
	Delay(3000000);
	GLCD_DisplayString(6, 3, __FI_LARGE, "MCB1700 Board");
	GLCD_DisplayString(8, 3, __FI_LARGE, "Right Joystick");
	GLCD_DisplayString(9, 5, __FI_LARGE, "To Exit");
	Delay(3000000);
}

int funcPhotoGallery (void) {
	
	int exitFlag = 1;
	
	for (;;) {
		// Display Photo Options
		menuPhotoGallery();
		Delay(10000000);

		// Select Photo
		int photoNum = photoSelector();
		
		if (photoNum == 0) {
			return 0;
		}
		
		// Display photo
		displayPhoto(photoNum);
	}
}