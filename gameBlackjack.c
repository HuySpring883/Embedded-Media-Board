#include <stdio.h>
#include <stdlib.h>
#include "KBD.h"
#include "GLCD.h"
#include <math.h>
#include <time.h>
#include "LPC17xx.h"

#define __FI_LARGE  1
#define __FI_SMALL 	0
#define DECK_SIZE 52

// menu.c
extern int Delay(uint32_t);
extern void displayOptions(void);

int funcBlackjack (void);
void menuBlackjack (void);
int selectionManagerBlackjack(void);
void selectionExecutorBlackjack (unsigned int tempLine);
int startBlackjack (void);
void playerTurn (void);
void dealerTurn (void);
int drawCard (void);
void winConditionBlackjack (void);
void printRulesBlackjack (void);
void shuffleDeck (void);
int continueMenu (void);

int selectionBlackjack;
int deck[DECK_SIZE];
int deckIndex = 0;
int playerHand = 0;
int dealerHand = 0;

int funcBlackjack (void) {
	
	int exitFlag = 1;
  
	menuBlackjack();
	selectionBlackjack = selectionManagerBlackjack();
	
	if (selectionBlackjack == 0) {
		GLCD_Clear(White);
		GLCD_DisplayString(5, 3, __FI_LARGE, " EXITING ");
		Delay(10000000);
		return 1;
	}
}

void menuBlackjack (void) {
	
	GLCD_Clear(White);
	GLCD_DisplayString(0, 3, __FI_LARGE, " Blackjack ");
	GLCD_DisplayString(4, 3, __FI_LARGE, "Start Game");
	GLCD_DisplayString(5, 3, __FI_LARGE, "Rules");
	GLCD_SetBackColor(Red);
	GLCD_DisplayString(6, 3, __FI_LARGE, "EXIT");
	GLCD_SetBackColor(White);
	Delay(3000000);
	
}

int selectionManagerBlackjack (void) {
	// Joystick Functionality selectionBlackjack Logic
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
					selectionExecutorBlackjack(tempLine);
					menuBlackjack();
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

void selectionExecutorBlackjack (unsigned int tempLine) {
	
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
			  startBlackjack();
				Delay(30000000);
				exitFlag = 1;
				break;
			case 5:
				printRulesBlackjack();
				Delay(30000000);
				exitFlag = 1;
				break;
			default:
				GLCD_Clear(White);
				GLCD_DisplayString(5, 0, __FI_LARGE, "Blackjack Error");
				Delay(30000000);
		}
	}
}

int startBlackjack (void) {
	
	int exitFlag = 0;
	
	while (exitFlag == 0) {
		GLCD_Clear(White);
		GLCD_DisplayString(0, 3, __FI_LARGE, " Blackjack ");
		GLCD_DisplayString(0, 0, __FI_LARGE, "Left Hit-Right Stand");
		GLCD_DisplayString(2, 2, __FI_LARGE, "Your Hand:");
		GLCD_DisplayString(3, 2, __FI_LARGE, "Dealer Hand:");
		playerTurn();
		dealerTurn();
		winConditionBlackjack();
		exitFlag = continueMenu();
	}
}

int continueMenu (void) {
	
	Delay(10000000);
	GLCD_Clear(White);
	GLCD_DisplayString(0, 3, __FI_LARGE, " Blackjack ");
	GLCD_DisplayString(3, 3, __FI_LARGE, "Press any dir");
	GLCD_DisplayString(4, 3, __FI_LARGE, "to Continue");
	Delay(20000000);
	
	while (1) {
		if (get_button() == KBD_DOWN || get_button() == KBD_LEFT
			|| get_button() == KBD_UP || get_button() == KBD_RIGHT) {
			return 1;
		}
	}
	
	Delay(20000000);
}

void printRulesBlackjack (void) {
	
	GLCD_Clear(White);
	GLCD_DisplayString(0, 3, __FI_LARGE, "Blackjack");
	GLCD_DisplayString(2, 0, __FI_LARGE, "1.Verse Dealer");
	GLCD_DisplayString(3, 0, __FI_LARGE, "2.Choose to Hit or Miss");
	GLCD_DisplayString(4, 0, __FI_LARGE, "3.Over 21 is loss/bust");
	GLCD_DisplayString(7, 2, __FI_LARGE, "Press Joystick");
	GLCD_DisplayString(8, 4, __FI_LARGE, "To Exit");
	Delay(30000000);
	
	int exitFlag = 0;
	
	while (exitFlag == 0) {
		if (get_button() == KBD_SELECT) {
			exitFlag = 1;
			GLCD_Clear(White);
		}
	}
}

void shuffleDeck (void) {
	
	int i, tempDeck, j;
	time_t time (time_t * t);
	// srand(time(NULL));
	
	for (i = 0; i < DECK_SIZE; i++) {
		if (i < 4) {
			deck[i] = i;
		} else if (i < 36) {
			deck[i] = (i / 4) + 2;
		} else {
			deck[i] = 10;
		}
	}
	
	for (i = DECK_SIZE-1; i > 0; i--) {
		j = rand() % (i + 1);
		tempDeck = deck[i];
		deck[i] = deck[j];
		deck[j] = tempDeck;
	}
	
	deckIndex = 0;
}

int drawCard (void) {

		if (deckIndex >= DECK_SIZE) {
			shuffleDeck();
		}
		
		return (rand() % 10) + 1;
		
}

void playerTurn (void) {
	
	char tempHand[20];
	
	while (1) {
		if (get_button() == KBD_LEFT) {
			int card = drawCard();
			playerHand += card;
			snprintf(tempHand, sizeof(tempHand), "Your Hand: %d      ", playerHand);
			Delay(20000000);
			GLCD_DisplayString(2, 2, __FI_LARGE, (unsigned char *)tempHand);
			
			if (playerHand > 21) {
				break;
			}
		}
		
		if (get_button() == KBD_RIGHT) {
			break;
		}
	}
}

void dealerTurn (void) {
	
	char tempHand[20];
	
	while (dealerHand < 18) {
		Delay(10000000);
		int card = drawCard();
		dealerHand += card;
		snprintf(tempHand, sizeof(tempHand), "Dealer Hand: %d       ", dealerHand);
		Delay(20000000);
		GLCD_DisplayString(3, 2, __FI_LARGE, (unsigned char *)tempHand);
	}
}

void winConditionBlackjack (void) {
	
	if (playerHand > 21) {
		GLCD_DisplayString(8, 0, __FI_LARGE, "Bust: Dealer Wins");
	} else if (dealerHand > 21 || playerHand > dealerHand) {
		GLCD_DisplayString(8, 0, __FI_LARGE, "Player Wins");
	} else {
		GLCD_DisplayString(8, 0, __FI_LARGE, "Dealer Wins");
	}
	Delay(20000000);
	
}