#include "KBD.h"
#include "GLCD.h"

#define __FI_LARGE  1
#define __FI_SMALL 	0

// menu.c
extern int Delay(uint32_t);
extern void displayOptions(void);

// .bimp files
#include "symbol_X.c"
#include "symbol_O.c"
#include "symbol_X_selected.c"
#include "symbol_O_selected.c"
#include "gameboard.c"

#define __FI_LARGE 1
#define __FI_SMALL 0

int selectionTicTacToe;
int player;
char board[3][3] = {{' ', ' ', ' '},{' ', ' ', ' '},{' ', ' ', ' '}};
int xCoord[3] = {70, 120, 170};
int yCoord[3] = {40, 90, 140};

// Functions
int funcTicTacToe (void);
void menuTicTacToe (void);
void printRulesTicTacToe (void);
int winConditionTicTacToe (void);
void printOutcome (void);
int startTicTacToe (void);
void drawGameboard (void);
void drawSymbol (void);
void updateGameboard (char playerInput);
int selectionManagerTicTacToe (void);
void selectionExecutorTicTacToe (unsigned int);
void drawHighlight (int x, int y);
void updateJoystickPosition (int x, int y, int direction);
void inputSymbol (int x, int y, char symbol);

void menuTicTacToe (void) {
	
	GLCD_Clear(White);
  GLCD_DisplayString(0, 3, __FI_LARGE, "Tic Tac Toe");
	GLCD_DisplayString(4, 3, __FI_LARGE, "Start Game");
	GLCD_DisplayString(5, 3, __FI_LARGE, "Rules");
	GLCD_SetBackColor(Red);
	GLCD_DisplayString(6, 3, __FI_LARGE, "EXIT");
	GLCD_SetBackColor(White);
  Delay(3000000);
	
}

int selectionManagerTicTacToe (void) {
	
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
					selectionExecutorTicTacToe(tempLine);
					menuTicTacToe();
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

void selectionExecutorTicTacToe (unsigned int tempLine) {
	
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
			  startTicTacToe();
				Delay(30000000);
				exitFlag = 1;
				break;
			case 5:
				printRulesTicTacToe();
				Delay(30000000);
				exitFlag = 1;
				break;
			default:
				GLCD_Clear(White);
				GLCD_DisplayString(5, 0, __FI_LARGE, "Tic Tac Toe Error");
				Delay(30000000);
		}
	}
}

int funcTicTacToe (void) {
	
  int exitFlag = 1;
  
	menuTicTacToe();
	selectionTicTacToe = selectionManagerTicTacToe();
	
	if (selectionTicTacToe == 0) {
		GLCD_Clear(White);
		GLCD_DisplayString(5, 3, __FI_LARGE, " EXITING ");
		Delay(10000000);
		return 1;
	}
}

int startTicTacToe (void) {
	
	int i, x = 0, y = 0;
	char playerInput;
  player = 1;
	char symbol = 'X';
	int joystickState ;
	
	drawGameboard();

	while (1) {
		joystickState = get_button();

//		if (joystickState == KBD_SELECT) {
//			// inputSymbol(x, y, symbol);
//			// drawSymbol();
//			char winner = winConditionTicTacToe();
//			
//			if (winner != ' ') {
//				if (winner == 'X') {
//					GLCD_DisplayString(9, 1, __FI_LARGE, "Player X Wins");
//				} else {
//					GLCD_DisplayString(9, 1, __FI_LARGE, "Player O Wins");
//				}
//				break;
//			}
//			if (symbol == 'X') {
//				symbol = 'O';
//			} else {
//				symbol = 'X';
//			}
//		} else {
//			GLCD_DisplayString(9, 0, __FI_LARGE, "Updating Position");
//			updateJoystickPosition(x, y, joystickState);
//		}
		GLCD_DisplayString(9, 0, __FI_LARGE, "Updating Position");
		updateJoystickPosition(x, y, joystickState);
	}

	return 0;

}

int winConditionTicTacToe (void) {
	
	int i;

	// Rows and columns
	for (i = 0; i < 3; i++){
		if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ') {
			return board[i][0];
		}
		if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ') {
			return board[0][i];
		}
	}

	// Diagonals
	if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ') {
		return board[0][0];
	}
	if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ') {
		return board[0][2];
	}
 
  	return ' ';
}

void printOutcome (void) {
	
	GLCD_Clear(White);
	GLCD_DisplayString(0, 3, __FI_LARGE, "Tic Tac Toe");
	GLCD_DisplayString(4, 4, __FI_LARGE, "WINNER");
	GLCD_DisplayString(9, 0, __FI_LARGE, "Press Joystick to EXIT");
	Delay(3000000);
	
	int exitFlag = 0;
	
	while (exitFlag == 0) {
		if (get_button() == KBD_SELECT) {
			exitFlag = 1;
			GLCD_Clear(White);                                                                                      
		}
	}
}

void printRulesTicTacToe (void) {
	
	GLCD_Clear(White);
	GLCD_DisplayString(0, 3, __FI_LARGE, "Tic Tac Toe");
	GLCD_DisplayString(2, 0, __FI_LARGE, "1.You first choose");
	GLCD_DisplayString(3, 0, __FI_LARGE, "2.Versing bot");
	GLCD_DisplayString(4, 0, __FI_LARGE, "3.Get 3 to win");
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

/* Draw the game board here */
void drawGameboard (void) {

	// GLCD Display: 320x240 pixels
  GLCD_Bitmap(70, 40, 150, 150, GAMEBOARD_TICTACTOE_PIXEL_DATA);
}

void inputSymbol (int x, int y, char symbol) {

	if (board[x][y] == ' ') {
		board[x][y] = symbol;
		if (symbol == 'X') {
			GLCD_Bitmap(xCoord[x], yCoord[y], 50, 50, SYMBOL_X_PIXEL_DATA);
		} else if (symbol == 'O') {
			GLCD_Bitmap(xCoord[x], yCoord[y], 50, 50, SYMBOL_O_PIXEL_DATA);
		}
	}
}

void drawSymbol (void) {

	int i, j;

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			if (board[i][j] == 'X') {
				GLCD_Bitmap(xCoord[j], yCoord[i], 50, 50, SYMBOL_X_SELECTED_PIXEL_DATA);
			} else if (board[i][j] == 'O') {
				GLCD_Bitmap(xCoord[j], yCoord[i], 50, 50, SYMBOL_O_SELECTED_PIXEL_DATA);
			}
		}
	}
}

void drawHighlight (int x, int y) {
	
	GLCD_Bitmap(xCoord[x], yCoord[y], 50, 50, SYMBOL_O_PIXEL_DATA);

}

void updateGameboard (char playerInput) {
 
// if (selectionTicTacToe == 1 && cells[1] == '1') {
//	 GLCD_Bitmap(70, 40, 50, 50, SYMBOL_X_PIXEL_DATA);
//	 cells[1] = playerInput;
// } else if (selectionTicTacToe == 2 && cells[2] == '2') {
//	 GLCD_Bitmap(120, 40, 50, 50, SYMBOL_X_PIXEL_DATA);
//   cells[2] = playerInput;
// } else if (selectionTicTacToe == 3 && cells[3] == '3') {
//	 GLCD_Bitmap(170, 40, 50, 50, SYMBOL_X_PIXEL_DATA);
//   cells[3] = playerInput;
// } else if (selectionTicTacToe == 4 && cells[4] == '4') {
//	 GLCD_Bitmap(70, 90, 50, 50, SYMBOL_X_PIXEL_DATA);
//   cells[4] = playerInput;
// } else if (selectionTicTacToe == 5 && cells[5] == '5') {
//	 GLCD_Bitmap(120, 90, 50, 50, SYMBOL_X_PIXEL_DATA);
//   cells[5] = playerInput;
// } else if (selectionTicTacToe == 6 && cells[6] == '6') {
//	 GLCD_Bitmap(170, 90, 50, 50, SYMBOL_X_PIXEL_DATA);
//   cells[6] = playerInput;
// } else if (selectionTicTacToe == 7 && cells[7] == '7') {
//	 GLCD_Bitmap(70, 140, 50, 50, SYMBOL_X_PIXEL_DATA);
//   cells[7] = playerInput;
// } else if (selectionTicTacToe == 8 && cells[8] == '8') {
//	 GLCD_Bitmap(120, 140, 50, 50, SYMBOL_X_PIXEL_DATA);
//   cells[8] = playerInput;
// } else if (selectionTicTacToe == 9 && cells[9] == '9') {
//	 GLCD_Bitmap(170, 140, 50, 50, SYMBOL_X_PIXEL_DATA);
//   cells[9] = playerInput;
// } else {
//   player--;
// }
 
}

//void updateJoystickPosition (int *x, int *y, int direction) {

//	if (direction == KBD_UP && *x > 0) {
//		*x--;
//	} else if (direction == KBD_DOWN && *x < 2) {
//		*x++;
//	} else if (direction == KBD_LEFT && *y > 0) {
//		*y--;
//	} else if (direction == KBD_RIGHT && *y < 2) {
//		*y++;
//	}

//}

void updateJoystickPosition (int x, int y, int joystickState) {
	
	unsigned int tempX = x;
	unsigned int tempY = y;
	unsigned int tempJoystickState = joystickState;
	int exitFlag = 0;
	
	while (exitFlag == 0) {
		switch(tempJoystickState) {
			case KBD_SELECT:
				tempJoystickState = NULL;
				drawSymbol();
				exitFlag = 1;
				break;
			case KBD_UP:
				if (tempJoystickState == KBD_UP && tempY > 0) {
					drawHighlight(tempX, tempY);
					drawSymbol();
					tempY--;
				}
				tempJoystickState = NULL;
				break;
			case KBD_DOWN:
				if (tempJoystickState == KBD_DOWN && tempY < 2) {
					drawHighlight(tempX, tempY);
					drawSymbol();
					tempY++;
				}
				tempJoystickState = NULL;
				break;
			case KBD_LEFT:
				if (tempJoystickState == KBD_LEFT && tempX > 0) {
					drawHighlight(tempX, tempY);
					drawSymbol();
					tempX--;
				}
				tempJoystickState = NULL;
				break;
			case KBD_RIGHT:
				if (tempJoystickState == KBD_RIGHT && tempX < 2) {
					drawHighlight(tempX, tempY);
					drawSymbol();
					tempX++;
				}
				tempJoystickState = NULL;
				break;
			default:
				drawHighlight(tempX, tempY);
				GLCD_DisplayString(8, 0, __FI_LARGE, "Drawing Symbol");
		}
	}
}






