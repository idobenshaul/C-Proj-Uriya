#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logic.h"
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

// copy's the array from the original to copy
void board_init(char*** board) {
	int i, j;
	for (i = 0; i < 7; i++) {
		for (j = 0; j < 7; j++) {
			(*board)[i][j] = "#";
		}
	}

}
int evalFunc(void* state) {
	char *** board = (char ***) state;
	int i = 0, j = 0;
	int catR, catC, mouseR, mouseC, cheeseR, cheeseC;
	char temp;

	for (i = 0; i < 7; i++) {
		for (j = 0; j < 7; j++) {
			temp = (*board)[i][j];
			switch (temp) {
			case 'M':
				mouseR = i;
				mouseC = j;
				break;
			case 'C':
				catR = i;
				catC = j;
				break;
			case 'P':
				cheeseR = i;
				cheeseC = j;
				break;
			}
		}
	}
	//char*** board =(char***) &(state);
	//printf("YUVAL\n");
	//printBoard(*board);
	printf("NOW ENTERING MORDOR W/ FOLLOWING BOARD\n");
	printBoard(*board);
	//printf("shit's about to get real:\n");
	//int a;
	//scanf("%d",a);
	int DCatMouse = findNeighborBFS(board, catR, catC, mouseR, mouseC);
	//int DCatMouse=findShortestPath(board, catR,catC,mouseR,mouseC);
	int DMouseCheese = findNeighborBFS(board, mouseR, mouseC, cheeseR, cheeseC);
	//int DCatMouse= ((catR-mouseR)*(catR-mouseR)+(catC-mouseC)*(catC-mouseC));
	//int DMouseCheese= ((cheeseR-mouseR)*(cheeseR-mouseR)+(cheeseC-mouseC)*(cheeseC-mouseC));
	//DCatMouse=DCatMouse*DCatMouse;
	//DMouseCheese=DMouseCheese*DMouseCheese;
	printf("the current board\n");
	printBoard(*board);
	printf("and the current eval =%d\n", DMouseCheese - DCatMouse);
	return DMouseCheese - DCatMouse;

}
findNeighborBFS(char***board, int srcR, int srcC, int destR, int destC) {
	///CHANGE NAMES, VERY CONFUSING!!
	int cnt = 199999;
	int a;
	//up
	findShortestPath(board, srcR, srcC, destR - 1, destC);
	//int ** dist=(int**)(returnDist());
	printf("src(%d,%d) to dest(%d,%d) FIRST NEIGHBOR\n", srcR, srcC, destR - 1,
			destC);
	if ((destR - 1) >= 0) {
		if ((*board)[destR - 1][destC] != 'W') {
			a = dist[destR - 1][destC];
			if (a < cnt) {
				cnt = a;
			}
		}
	}
	//right
	printf("src(%d,%d) to dest(%d,%d) SECOND NEIGHBOR\n", srcR, srcC, destR,
			destC + 1);
	if ((destC + 1) <= 6) {
		if ((*board)[destR][destC + 1] != 'W') {
			a = dist[destR][destC + 1];
			if (a < cnt) {
				cnt = a;
			}
		}
	}
	//down
	printf("src(%d,%d) to dest(%d,%d) THIRD NEIGHBOR\n", srcR, srcC, destR + 1,
			destC);
	if ((destR + 1) <= 6) {
		if ((*board)[destR + 1][destC] != 'W') {
			a = dist[destR + 1][destC];
			if (a < cnt) {
				cnt = a;
			}
		}
	}
	//left
	printf("src(%d,%d) to dest(%d,%d) FOURTH NEIGHBOR\n", srcR, srcC, destR,
			destC - 1);
	//int ork_I;
	//printf("they're taking the hobbits to eizengard!! (Lagolas)\n");
	//scanf("%d", &ork_I);
	if ((destC - 1) >= 0) {
		if ((*board)[destR][destC - 1] != 'W') {
			a = dist[destR][destC - 1];
			if (a < cnt) {
				cnt = a;
			}
		}
	}
	return cnt + 1;

}
void copyArray(char *** original, char ***copy) {    ////////for int[6][7]
	int i = 0, j = 0;
	for (i = 0; i < 7; i++) {
		for (j = 0; j < 7; j++) {
			(*copy)[i][j] = (*original)[i][j];
		}
	}
	//printf("this is end of copy board");
	//printBoard(*copy);
}

//returns a list with all possible children of the current state
ListRef getChildrenStates(void* state) {
	//printf ("getChildren started\n");
	//int turn=countTurn(state);//
	char ***board = (char ***) state;
	int i = 0, j = 0;
	ListRef children = newList(NULL);
	char** curState;
	Child** states[4];
	Child* childs[4];
	Child* curChild;
	//int realRow,realCol;
	for (i = 0; i < 4; i++) {
		curState = (char**) malloc(7 * sizeof(char*));
		for (j = 0; j < 7; j++) {
			curState[j] = (char*) malloc(7 * sizeof(char));
		}
		board_init(&curState);
		copyArray(board, &curState);
		updateBoardStatus(curState);
		//printf("printing the %d'th direction before move!!\n", i+1);
		//printBoard(curState);
		childs[i] = (Child*) malloc(sizeof(Child));
		if (move(&curState, i + 1) == 1) {
			printf("printing the %d'th direction\n", i + 1);
			printBoard(curState);
			//childs[i]=(Child*)malloc(sizeof(Child));
			childs[i]->state = curState;
			//childs[i]->result.value=evalFunc(&(childs[i]->state));
			childs[i]->result.index = i + 1;
			//states[i]=(Child**)(&childs[i]);
		} else {

			childs[i]->state = NULL;
		}
		states[i] = (Child**) (&childs[i]);
	}

	//printf("printing the board after the loop\n");
	//printBoard(*board);
	updateBoardStatus(*board);
	for (i = 0; i < 4; i++) {
		Child a = *(*(states[i]));
		if (a.state != NULL) {
			append(children, (Child*) *(states[i]));
			printf("now printing children, %d\n", i + 1);
			printf("%d\n", ((Child*) *(states[i]))->result.value);
		}
	}

	return children;
}

void switchTurn() {
	if (!strcmp(turn, "cat")) {
		sprintf(turn, "mouse");
	} else
		sprintf(turn, "cat");

}
void switchIsOriginalTurn() {
	if (isOriginalTurn == 0) {
		isOriginalTurn = 1;
	} else {
		isOriginalTurn = 0;
	}
}
struct MiniMaxResult wrapGetBestChild(int actualCatRow, int actualCatCol,
		int actualMouseRow, int actualMouseCol, void* state,
		unsigned int maxDepth, ListRef (*getChildren)(void* state),
		FreeFunc freeState, int (*evaluate)(void* state), int isMaxPlayer) {

	struct MiniMaxResult temp;
	temp = getBestChild(state, maxDepth, getChildren, freeState, evaluate,
			isMaxPlayer);
	mouseRow = actualMouseRow;
	mouseCol = actualMouseCol;
	catRow = actualCatRow;
	catCol = actualCatCol;
	//makeOriginalBoard(board);
	/////////////////////////////////////////////////////////////////////////////

	if (isOriginalTurn == 0) {
		switchTurn();
		switchIsOriginalTurn();
	}
	return temp;
}
int consoleMode(int argc, char* args[]) {

	setvbuf(stdout, NULL, _IONBF, 0);
	gameOptions game = { 0, 1, 4, 0 };
	int moveWork = -17, direction = -1;
	char temp[7];
	char** board; //7*7 board
	board = loadGame(1);
	printf("result %d\n", updateBoardStatus(board));
	printf("cat %d %d", catRow, catCol);
	while (!updateBoardStatus(board)) {
		///we'll use Cat as maxPlayer:1, and Mouse as maxPlayer:0
		///same goes for comp1 and comp2
		int actualCatRow = catRow, actualCatCol = catCol, actualMouseRow =
				mouseRow, actualMouseCol = mouseCol;
		printBoard(board);
		isOriginalTurn = 1;

		//printf("cat human : %d, mouse human : %d");
		if ((game.cat_human == 1 && strcmp(turn, "cat") == 0) //if it's human turn
		|| (game.mouse_human == 1 && strcmp(turn, "mouse") == 0)) {
			printf("%s's turn- type a move please (U/D/L/R): \n", turn);
			scanf("%s", temp);

			switch (temp[0]) {
			case 'L':
				direction = LEFT;
				break;
			case 'R':
				direction = RIGHT;
				break;
			case 'U':
				direction = UP;
				break;
			case 'D':
				direction = DOWN;
				break;

			} //end switch
		} //end human move
		else { //machine move
			if (strcmp(turn, "cat") == 0) {
				direction = wrapGetBestChild(actualCatRow, actualCatCol,
						actualMouseRow, actualMouseCol, &board, game.cat_skill,
						getChildrenStates, free, evalFunc, 1).index;
			} else { //(strcmp(turn, "mouse")==0)
				direction =
						wrapGetBestChild(actualCatRow, actualCatCol,
								actualMouseRow, actualMouseCol, &board,
								game.mouse_skill, getChildrenStates, free,
								evalFunc, 1).index;
			}
		}
		//TODO - is this neccesary?
		//makeOriginalBoard(board);

		if (move(&board, direction)) {
			switchTurn();
		}


	}
	switch (updateBoardStatus(board)) {
	case 0:
		printf("Game over. tie.\n");
		break;
	case 1:
		printf("Game over. Mouse wins.\n");
		break;
	case 2:
		printf("Game over. Cat wins. \n");
		break;
	}
	printBoard(board);
	freeBoard(board);
	return 0;
}

int isLegalMove(char*** board, int srcRow, int srcCol, int direction) {
	//returns 1 if desired move is legal

	if (direction == LEFT && srcCol > 0 && (*board)[srcRow][srcCol - 1] == '#')
		return 1;
	if (direction == RIGHT && srcCol < 6 && (*board)[srcRow][srcCol + 1] == '#')
		return 1;
	if (direction == DOWN && srcRow < 6 && (*board)[srcRow + 1][srcCol] == '#')
		return 1;
	if (direction == UP && srcRow > 0 && (*board)[srcRow - 1][srcCol] == '#')
		return 1;
	return 0;

}

int move(char*** board, int direction) {
//returns 1 if move was valid and made, 0 on fail;
	if (!strcmp(turn, "cat")) {
		if (!isLegalMove(board, catRow, catCol, direction)) {
			return 0;
		}
	}
	if (!strcmp(turn, "mouse")) {
		if (isLegalMove(board, mouseRow, mouseCol, direction) != 1) {
			return 0;
		}
	}
	if (!strcmp(turn, "cat")) {
		if (direction == LEFT && catCol > 0
				&& (*board)[catRow][catCol - 1] != 'W'
				&& (*board)[catRow][catCol - 1] != 'P') {
			(*board)[catRow][catCol] = '#';
			catCol--;
			(*board)[catRow][catCol] = 'C';
			return 1;
		}
		if (direction == RIGHT && catCol < 6
				&& (*board)[catRow][catCol + 1] != 'W'
				&& (*board)[catRow][catCol + 1] != 'P') {
			(*board)[catRow][catCol] = '#';
			catCol++;
			(*board)[catRow][catCol] = 'C';
			return 1;
		}
		if (direction == DOWN && catRow < 6
				&& (*board)[catRow + 1][catCol] != 'W'
				&& (*board)[catRow + 1][catCol] != 'P') {
			(*board)[catRow][catCol] = '#';
			catRow++;
			(*board)[catRow][catCol] = 'C';
			return 1;
		}
		if (direction == UP && catRow > 0 && (*board)[catRow - 1][catCol] != 'W'
				&& (*board)[catRow - 1][catCol] != 'P') {
			(*board)[catRow][catCol] = '#';
			catRow--;
			(*board)[catRow][catCol] = 'C';
			return 1;
		}
	}
	if (!strcmp(turn, "mouse")) {
		if (direction == LEFT && mouseCol > 0
				&& (*board)[mouseRow][mouseCol - 1] != 'W') {
			(*board)[mouseRow][mouseCol] = '#';
			mouseCol--;
			(*board)[mouseRow][mouseCol] = 'M';
			return 1;
		}
		if (direction == RIGHT && mouseCol < 6
				&& (*board)[mouseRow][mouseCol + 1] != 'W') {
			(*board)[mouseRow][mouseCol] = '#';
			mouseCol++;
			(*board)[mouseRow][mouseCol] = 'M';
			return 1;
		}
		if (direction == DOWN && mouseRow < 6
				&& (*board)[mouseRow + 1][mouseCol] != 'W') {
			(*board)[mouseRow][mouseCol] = '#';
			mouseRow++;
			(*board)[mouseRow][mouseCol] = 'M';
			return 1;
		}
		if (direction == UP && mouseRow > 0
				&& (*board)[mouseRow - 1][mouseCol] != 'W') {
			(*board)[mouseRow][mouseCol] = '#';
			mouseRow--;
			(*board)[mouseRow][mouseCol] = 'M';
			return 1;
		}
	}
	return 0;
}

void freeBoard(char** board) {
	if (board == NULL)
		return;
	int i = 0;
	for (i = 0; i < 7; i++) {
		free(board[i]);
	}
	free(board);

}
///NEED TO DO: the idea is to get catCol... and return a board w/ the objects in the correct positions
void makeOriginalBoard(char** board) {
	int i = 0, j = 0;
	char temp;
	for (i = 0; i < 7; i++) {
		for (j = 0; j < 7; j++) {
			temp = board[i][j];
			switch (temp) {
			case 'M':
				if (i != mouseRow || j != mouseCol) {
					board[i][j] = '#';
					board[mouseRow][mouseCol] = 'M';
				}
				break;
			case 'C':
				if (i != catRow || j != catCol) {
					board[i][j] = '#';
					board[catRow][catCol] = 'C';
				}
				break;
			}
		}
	}
	printf("printing board in makeOriginalBoard\n");
	printBoard(board);
	//board[mouseRow][mouseCol]="M";
	//board[catRow][catCol]="C";
}
int updateBoardStatus(char** board) {
//scan board for locations of mouse,cat,cheese
//update variables accordingly and return 1 if mouse wins, 2 if cat wins
	int i = 0, j = 0;

	char temp;
	for (i = 0; i < 7; i++) {
		for (j = 0; j < 7; j++) {
			temp = board[i][j];
			switch (temp) {
			case 'M':
				mouseRow = i;
				mouseCol = j;
				break;
			case 'C':
				catRow = i;
				catCol = j;
				break;
			case 'P':
				cheeseRow = i;
				cheeseCol = j;
				break;
			}
		}
	}
	if ((abs(cheeseRow - mouseRow) + abs(cheeseCol - mouseCol)) < 2)
		return 1;
	if ((abs(catRow - mouseRow) + abs(catCol - mouseCol)) < 2)
		return 2;
	else
		return 0;
}

void printBoard(char** board) {
	int i = 0, j = 0;
	char temp;
	printf("=============\n\n");
	if (board == NULL)
		return;
	for (i = 0; i < 7; i++) {
		for (j = 0; j < 7; j++) {
			temp = board[i][j];
			if (temp == '#')
				temp = '-';
			printf("%c ", temp);
		}
		printf("\n");
	}
//printf ("end printboard");
}

char** loadGame(int gameNum) {
//loads the game in "world_gamenum.txt" - allocs a board array first, and then loads the game world into it
	char temp = '0' + gameNum;
//printf ("%c", temp);
	int i = 0;
	char filename[20] = "worlds/world_#.txt";
	puts(filename);
	filename[13] = temp;

	if (!gameNum) {
		sprintf(filename, "worlds/template.txt");
	}

	char** board = calloc(7, sizeof(char*));

	for (i = 0; i < 7; i++) {
		board[i] = calloc(7, sizeof(char));
	}

//puts (filename);

	FILE * map = fopen(filename, "r");
	if (map == NULL) {
		exit(0);
	}
	fscanf(map, "%d\n", &max_turns);
	fscanf(map, "%s\n", turn);

	for (i = 0; i < 7; i++) {
		fscanf(map, "%s\n", board[i]);
	}

//printBoard(board);
	fclose(map);

	return board;
}

int checkBoard(char** board) {
	printBoard(board);
	//returns 0 if board is legal
	int mouse_flag = 0, cat_flag = 0, cheese_flag = 0;
	int i = 0, j = 0;
	char temp;

	for (i = 0; i < 7; i++) {
		for (j = 0; j < 7; j++) {
			temp = board[i][j];
			//printf("%c%c ", temp , board[i][j]);
			if (temp == 'C') {
				if (!cat_flag)
					cat_flag = 1;
			}
			if (temp == 'M') {
				if (!mouse_flag)
					mouse_flag = 1;
			}
			if (temp == 'P') {
				if (!cheese_flag)
					cheese_flag = 1;
			}
		}
	}
	//puts( "");
	if (!cheese_flag) {
		return CHEESE_MISSING;
	} else if (!cat_flag) {
		return CAT_MISSING;
	} else if (!mouse_flag) {
		return MOUSE_MISSING;
	}
	return GOOD_WORLD;
}

int saveGame(char** board, int gamenum) {
	if (gamenum > 5 || gamenum < 0)
		exit(0);

//printf("%s", board[6]);
	int i = 0;
	char temp = '0' + gamenum;
	char filename[20] = "worlds/world_#.txt";
	filename[13] = temp;

	FILE * map = fopen(filename, "w");
	if (map == NULL) {
		exit(0);
	}

	fprintf(map, "%d\n", max_turns);
	fprintf(map, "%s\n", turn);
	for (i = 0; i < 7; i++) {
		fprintf(map, "%s\n", board[i]);
	}

	return fclose(map);
}
