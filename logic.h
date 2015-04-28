/*
 * logic.h
 *
 *  Created on: Apr 19, 2015
 *      Author: ub
 */

#ifndef LOGIC_H_
#define LOGIC_H_

#include "ListUtils.h"
#include "MiniMax.h"
#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4
int mouseRow, mouseCol, catRow, catCol, cheeseX, cheeseY;

void board_init(char*** board);
int evalFunc(void* state);
void switchTurn();
char** loadGame (int gamenum);
int move (char*** board, int direction);
void freeBoard(char** board);
int updateGameStatus (char** board);
void printBoard(char** board);
void copyArray(char *** original, char***copy);
char turn[7];//toggle cat's turn.
int max_turns ; //max num of turns.
int turnCounter;
int evalFunc(void* state);
ListRef getChildrenStates (void* state);
int checkBoard(char** board);
int saveGame(char** board, int gamenum);

struct gameOptions {
	int cat_human;
	int mouse_human;
	int cat_skill;
	int mouse_skill;
};
typedef struct gameOptions gameOptions;

#endif /* LOGIC_H_ */
