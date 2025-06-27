#include "KBD.h"
#include "GLCD.h"

#define __FI_LARGE  1
#define __FI_SMALL 	0

// menu.c
extern int Delay(uint32_t);
extern void displayOptions(void);

// gameTicTacToe.c
extern int funcTicTacToe (void);

// gameBlackJack.c
extern int funcBlackjack (void);

// Functions
int selectionManager (void);
void loadingGame (unsigned int tempLine);
int funcGameMenu (void);
void displayGameMenu (void);

int selectionManager (void) {
	
	// Joystick Functionality Selection Logic
	unsigned int tempLine = 4;
	unsigned int tempJoyStickState = NULL;
	
	for (;;) {
		switch(get_button()) {
			case KBD_SELECT:
				tempJoyStickState = NULL;
				GLCD_DisplayString(tempLine, 1, __FI_LARGE, "o");
				if (tempLine >= 6) {
					return 1;
				} else {
					loadingGame(tempLine);
					displayGameMenu();
				}
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

void loadingGame(unsigned int tempLine) {
	
	GLCD_Clear(White);
	GLCD_SetBackColor(White);
	GLCD_SetTextColor(Black);
	GLCD_DisplayString(3, 6, __FI_LARGE, "Loading...");
	Delay(10000000);
	
	// Decode Functionality Selected
	int exitFlag = 0;
	
	while (exitFlag == 0) {
		switch(tempLine) {
			case 4:
				GLCD_DisplayString(5, 3, __FI_LARGE, "Tic Tac Toe");
			  funcTicTacToe();
				Delay(30000000);
				exitFlag = 1;
				break;
			case 5:
				GLCD_DisplayString(5, 3, __FI_LARGE, "Blackjack");
				funcBlackjack();
				Delay(30000000);
				exitFlag = 1;
				break;
			default:
				GLCD_DisplayString(5, 1, __FI_LARGE, "Game Menu Error");
				Delay(30000000);
		}
	}
}

void displayGameMenu (void) {
	
	GLCD_Clear(White);
	Delay(3000000);
	GLCD_DisplayString(0, 3, __FI_LARGE, " Game Menu ");
	Delay(3000000);
	GLCD_SetBackColor(Yellow);
	GLCD_DisplayString(4, 3, __FI_LARGE, " Tic Tac Toe    ");		// Tic-Tac-Toe
	GLCD_DisplayString(5, 3, __FI_LARGE, " Blackjack ");			// Blackjack
	GLCD_SetBackColor(Red);
	GLCD_DisplayString(6, 3, __FI_LARGE, " EXIT ");							// Exit option
	GLCD_SetBackColor(White);
	GLCD_DisplayString(25, 10, __FI_SMALL, " Use Joystick To Navigate ");		// Info
	GLCD_DisplayString(26, 7, __FI_SMALL, " Press Joystick to Select ");		// Info Conti.
	Delay(3000000);
	
}

int funcGameMenu (void) {
	
	int selection = 1;
	
	displayGameMenu();
	selection = selectionManager();
	
	if (selection == 0) {
		GLCD_Clear(White);
		GLCD_DisplayString(5, 3, __FI_LARGE, " EXITING ");
		Delay(10000000);
		return 1;
	}
}