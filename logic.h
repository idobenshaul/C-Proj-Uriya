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

#define GOOD_WORLD 0
#define CAT_MISSING 1
#define MOUSE_MISSING 2
#define CHEESE_MISSING 3


int mouseRow, mouseCol, catRow, catCol, cheeseRow, cheeseCol;

int consoleMode ();
void board_init(char*** board);
int evalFunc(void* state);
void switchTurn();
char** loadGame (int gamenum);
int move (char*** board, int direction);
void freeBoard(char** board);
int updateGameStatus(char** board);
void printBoard(char** board);
void copyArray(char *** original, char***copy);
char turn[7];//toggle cat's turn.
int isOriginalTurn;
int max_turns ; //max num of turns.
int turnCounter;
int evalFunc(void* state);
ListRef getChildrenStates (void* state);
int checkBoard(char** board);
int saveGame(char** board, int gamenum);
void makeOriginalBoard(char** board);
void switchIsOriginalTurn();
int isLegalMove(char*** board, int srcRow, int srcCol, int direction);
int findNeighborBFS(char***board, int mouseR,int mouseC,int cheeseR,int cheeseC);
//int cntFailedMoves=0;
//int badIndex;
int visited[7][7];
int dist[7][7];
struct MiniMaxResult wrapGetBestChild(int actualCatRow, int actualCatCol,int actualMouseRow,int  actualMouseCol,
		void* state, unsigned int maxDepth,
		        ListRef (*getChildren) (void* state),FreeFunc freeState,
		        int (*evaluate) (void* state), int isMaxPlayer);
struct gameOptions {
	int cat_human;
	int mouse_human;
	int cat_skill;
	int mouse_skill;
};
typedef struct gameOptions gameOptions;

#endif /* LOGIC_H_ */
