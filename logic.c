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
void board_init(char*** board){
    int i,j;
    for(i=0;i<7;i++){
        for(j=0;j<7;j++){
            (*board)[i][j]="#";
        }
    }

}
int evalFunc(void* state){
    char *** board=(char ***)state;
    int i = 0, j = 0;
    int catR,catC,mouseR,mouseC,cheeseR,cheeseC;
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

    int DCatMouse= ((catR-mouseR)*(catR-mouseR)+(catC-mouseC)*(catC-mouseC));
    int DMouseCheese= ((cheeseR-mouseR)*(cheeseR-mouseR)+(cheeseC-mouseC)*(cheeseC-mouseC));
    return DMouseCheese-DCatMouse;

}
void copyArray(char *** original, char ***copy){////////for int[6][7]
	int i=0,j=0;
	for(i=0;i<7;i++){
		for(j=0;j<7;j++){
			(*copy)[i][j]=(*original)[i][j];}}
    //printf("this is end of copy board");
    //printBoard(*copy);
}


//returns a list with all possible children of the current state
ListRef getChildrenStates (void* state){
	//printf ("getChildren started\n");
	//int turn=countTurn(state);//
	char ***board= (char ***) state;
	int i=0,j=0;
	ListRef children=newList(NULL);
    char** curState;
    Child** states[4];
    Child* childs[4];
    Child* curChild;
    //int realRow,realCol;
    for(i=0;i<4;i++){
        curState=(char**)malloc(7*sizeof(char*));
        for(j=0;j<7;j++){
            curState[j]=(char*)malloc(7*sizeof(char));
        }
        board_init(&curState);
        copyArray(board,&curState);
        updateGameStatus(curState);
        //printf("printing the %d'th direction before move!!\n", i+1);
        //printBoard(curState);
        childs[i]=(Child*)malloc(sizeof(Child));
        if(move(&curState,i+1)==1){
            printf("printing the %d'th direction\n", i+1);
            printBoard(curState);
            //childs[i]=(Child*)malloc(sizeof(Child));
            childs[i]->state=curState;
            childs[i]->result.value=evalFunc(&(childs[i]->state));
            childs[i]->result.index=i+1;
            //states[i]=(Child**)(&childs[i]);
        }
        else{

            childs[i]->state=NULL;
        }
        states[i]=(Child**)(&childs[i]);
    }

    //printf("printing the board after the loop\n");
    //printBoard(*board);
    updateGameStatus(*board);
    for(i=0;i<4;i++){
        Child a=*(*(states[i]));
        if(a.state!=NULL){
            append(children, (Child*)*(states[i]));
            printf("now printing children, %d\n",i+1);
            printf("%d\n",((Child*)*(states[i]))->result.value);
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
void switchIsOriginalTurn(){
    if (isOriginalTurn==0) {
            isOriginalTurn=1;
        }
    else{
        isOriginalTurn=0;
    }
}
struct MiniMaxResult alphabeta(node, depth, α, β, maximizingPlayer){
      if depth = 0 or node is a terminal node
          return the heuristic value of node
      if maximizingPlayer
          v := -∞
          for each child of node
              v := max(v, alphabeta(child, depth - 1, α, β, FALSE))
              α := max(α, v)
              if β ≤ α
                  break (* β cut-off *)
          return v
      else
          v := ∞
          for each child of node
              v := min(v, alphabeta(child, depth - 1, α, β, TRUE))
              β := min(β, v)
              if β ≤ α
                  break (* α cut-off *)
          return v
}
int main(int argc, char* args[]) {

	setvbuf(stdout, NULL, _IONBF, 0);
	gameOptions game = { 0, 1, 7, 0 };
    int moveWork=-17;
	char temp[7];
	char** board; //7*7 board
	board = loadGame(1);
	printf("result %d\n", updateGameStatus(board));
	printf("cat %d %d", catRow, catCol);
	while (!updateGameStatus(board)) {
        ///we'll use Cat as maxPlayer:1, and Mouse as maxPlayer:0
        ///same goes for comp1 and comp2
        int actualCatRow=catRow, actualCatCol=catCol,actualMouseRow=mouseRow, actualMouseCol=mouseCol;
		printBoard(board);
		isOriginalTurn=1;

		//printf("cat human : %d, mouse human : %d");
		if ((game.cat_human==1 && strcmp(turn, "cat")==0)//if it's human turn
				|| (game.mouse_human==1 && strcmp(turn, "mouse")==0)) {
			printf("%s's turn- type a move please (U/D/L/R): \n", turn);
			scanf("%s", temp);
		} //end human move
		else { //machine move
			   //TODO - implement machine move - the result should be put into temp[0] as a character implying direction - L/R/D/U
            /////////////////////////////////////////////////////////
            if(strcmp(turn, "cat")==0){
                temp[0]=getBestChild(&board,game.cat_skill, getChildrenStates, free, evalFunc, 1).index;
            }
            else{//(strcmp(turn, "mouse")==0)
                temp[0]=getBestChild(&board,game.mouse_skill, getChildrenStates, free, evalFunc, 0).index;
            }


            ///////////////////////////////////////////
            /*
            if (!game.cat_human && !game.mouse_human){
                if(strcmp(turn, "cat")){
                    temp[0]=getBestChild(&board,game.cat_skill, getChildrenStates, free, evalFunc, 1).index;

                }
                else{
                    temp[0]=getBestChild(&board,game.mouse_skill, getChildrenStates, free, evalFunc, 0).index;

                }
            }
            if(!game.cat_human && game.mouse_human){//cat is Comp, mouse is human, Comp turn
                temp[0]=getBestChild(&board,game.cat_skill, getChildrenStates, free, evalFunc, 1).index;

            }
            else{//mouse is Comp, cat is human, Comp turn
                temp[0]=getBestChild(&board,game.mouse_skill, getChildrenStates, free, evalFunc, 0).index;

            }*/
		}
		mouseRow=actualMouseRow;
		mouseCol=actualMouseCol;
		catRow=actualCatRow;
		catCol=actualCatCol;
		makeOriginalBoard(board);
		/////////////////////////////////////////////////////////////////////////////
		if(isOriginalTurn==0){
            switchTurn();
            switchIsOriginalTurn();
		}
		///assigning bad index
		/*if(temp[0]<5){
            badIndex=temp[0];
		}
		else{//person entered, why do we need it ?
            switch(temp[0]){
            case 85:
                badIndex=1;
                break;
            case 82:
                badIndex=2;
                break;
            case 68:
                badIndex=3;
                break;
            case 76:
                badIndex=4;
                break;
            }
		}*/


		switch (temp[0]) {
		case 'L':
        case 4:
			moveWork=move(&board, LEFT);
			break;
		case 'R':
        case 2:
			moveWork=move(&board, RIGHT);
			break;
			//printf("printing the board level two after choosing to move right\n");
			//printBoard(board);

		case 'U':
        case 1:
			moveWork=move(&board, UP);
			break;

		case 'D':
        case 3:
			moveWork=move(&board, DOWN);
			break;

		} //end switch

		if(moveWork==0){
                //cntFailedMoves++;
                switchTurn();
		}
		/*if(moveWork==1){
            cntFailedMoves=0;
		}*/
		switchTurn();
		///IMPORTANT: notice that there are 3 different types of switch turns present:
		/* 1: to bring the turn back if it got altered in the
		recursion, 2: to make the turn stay the same if the move we attempted
		to do was illegal, and 3: to switch turn at the end of the loop*/
	}
	switch (updateGameStatus(board)) {
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
//saveGame(board, 3);

	freeBoard(board);
	return 0;
}

int move(char*** board, int direction) {
//returns 1 if move was valid and made, 0 on fail;
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
void makeOriginalBoard(char** board){
    int i=0,j=0;
    char temp;
    for(i=0;i<7;i++){
        for(j=0;j<7;j++){
            temp = board[i][j];
			switch (temp) {
			case 'M':
			    if(i!=mouseRow || j!=mouseCol){
                    board[i][j]="#";
                    board[mouseRow][mouseCol]="M";
			    }
			    break;
			case 'C':
			    if(i!=catRow || j!=catCol){
                    board[i][j]="#";
                    board[catRow][catCol]="C";
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
int updateGameStatus(char** board) {
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
				cheeseX = i;
				cheeseY = j;
				break;
			}
		}
	}
	if ((abs(cheeseX - mouseRow) + abs(cheeseY - mouseCol)) < 2)
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

char** loadGame(int gamenum) {
//loads the game in "world_gamenum.txt"
	char temp = '0' + gamenum;
//printf ("%c", temp);
	int i = 0;
	char filename[20] = "world_#.txt";
	filename[6] = temp;

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
	int mouse_flag = 0, cat_flag = 0, cheese_flag = 0;
	int i = 0, j = 0;
	char temp;
	for (i = 0; i < 7; i++) {
		for (j = 0; j < 7; j++) {
			temp = board[j][j];
			if (temp == 'C') {
				if (cat_flag)
					return 0;
				else
					cat_flag = 1;
			}
			if (temp == 'M') {
				if (mouse_flag)
					return 0;
				else
					mouse_flag = 1;
			}
			if (temp == 'P') {
				if (cheese_flag)
					return 0;
				else
					cheese_flag = 1;
			}
		}
	}
	return 1;
}

int saveGame(char** board, int gamenum) {
	if (gamenum > 5 || gamenum < 0)
		exit(0);

//printf("%s", board[6]);
	int i = 0;
	char temp = '0' + gamenum;
	char filename[20] = "world_#.txt";
	filename[6] = temp;

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
