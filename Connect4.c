#define FIRSTCOMMAND if (firstcommand) {printf ("Error: first command must be set_number_steps.\n");} else{
#define GAMEOVER if(gameover) {printf ("Error: game is over.\n");} else{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "MiniMax.h"
#include "logic.h"


void copyArray(int *** original, int *** copy);
ListRef getChildren (void* state);
int countTurn (void* state);
int availPlace(void* state, int col);
void insertDisc (int row, int col, int*** board, int player);

int is_empty(const char *s);
void board_init(int ***board);
int evalFunc(void* state);
int isFull(int ** board);
int getHeight(void* state, int col);


///a function for printing the board


//checks if there are empty spaces in the board
int isFull(int ** board){
	int i=0,j=0;
	for(i=0;i<6;i++){
		for(j=0;j<7;j++){
			if(board[i][j]==0){
				return 0;
			}
		}
	}
	return 1;
}

//inserts a disc into board[row][col], according to which player's turn it is
void insertDisc (int row, int col, int*** board, int player){
	//player: 1 human , -1 computer
	if (row>0 ){
		if ((*board)[row-1][col]!=0)
			(*board)[row][col]=player;
	}
	else if (row==0){
		(*board)[row][col]=player;
	}
	else{
		return;
	}

}

//checks if the string is empty
int is_empty(const char *s) {
  while (*s != '\0') {
    if (!isspace(*s))
      return 0;
    s++;
  }
  return 1;
}

//initializes board to zeros's
void board_init(int *** board){
	int i,j;

	for (i=0; i<6; i++){
		for (j=0;j<7;j++){
			(*board)[i][j]=0;
		}
	}
}

//counts number of pieces for each side, and determines who's turn it is
int countTurn (void* state){
	int ***board= (int ***) state;
	int i=0,j=0, sum=0;
	for(i=0;i<6;i++){
		for(j=0;j<7;j++){
			#ifdef DEBUG
				printf ("sum is: %d, i: %d, j: %d \n", sum, i ,j );
			#endif
			sum+=(*board)[i][j];}}
	return ((sum==1)?-1:1);
}
//checks if specific col is available for adding a disc


//checks the height of the board on a certain col
int getHeight(void* state, int col){

	int ***board= (int ***) state;
	int j=5;
	//printf("getheight start, col: %d\n", col);

	while((*board)[j][col]==0){
		#ifdef DEBUG
			printf("j: %d \n",j);
		#endif
		j--;
		if(j==-1){
			break;
		}

	}
	//printf("result of getHeight: j: %d \n",j+1);
	return j+1;
}

// copy's the array from the original to copy
void copyArray(int *** original, int ***copy){////////for int[6][7]
	int i=0,j=0;
	for(i=0;i<6;i++){
		for(j=0;j<7;j++){
			(*copy)[i][j]=(*original)[i][j];}}
}
//returns a list with all possible children of the current state
ListRef getChildren (void* state){
	//printf ("getChildren started\n");
	int turn=countTurn(state);
	int ***board= (int ***) state;
	int i=0,j=0;
	ListRef children=newList(NULL);
	int ** curState;
	Child* curChild;

	for (i=0;i<7;i++){
        //allocation of state for i th child

        curState= (int**) malloc (6 * sizeof(int*));
        for (j=0; j< 6; j++){
            curState[j]= (int*)malloc(7 *sizeof(int));
        }
        board_init(&curState);
        copyArray(board, &curState);
        //curState[getHeight(state,i)][i]=turn;
        move(&curState,i);
        //building a node with the hypothetic move and appending it to a list of children
        curChild = (Child*)malloc(sizeof(Child));
        curChild->state= curState;
        curChild->result.value=(!strcmp(turn, "cat"))?199999:-199999;
        curChild->result.index=i;
        append(children, curChild);

	}

	return children;
}

//eval function that computes evaluation for state in accordance to the instructions
int evalFunc(void* state){
	char *** board=(char ***)state;
	int score = 0;
    int DCatMouse= ((catRow-mouseRow)*(catRow-mouseRow)+(catCol-mouseCol)*(catCol-mouseCol));
    int DMouseCheese= ((cheeseX-mouseRow)*(cheeseX-mouseRow)+(cheeseY-mouseCol)*(cheeseY-mouseCol));
    return DCatMouse-DMouseCheese;
}

int main(){
	int ** board;
	int heights[7]= {0,0,0,0,0,0,0};
	int i=0, num_steps=0, param=-1,firstcommand=1,gameover=0, overflowflag=0, score=0,compIndex=1000;
	char inp[43]="aaa", *QUIT="quit", command[43]="AA";


	board= (int**) malloc (6 * sizeof(int*));
	for (i=0; i< 6; i++){
		board[i]= (int*)malloc(7 *sizeof(int));
	}
	board_init(&board);
	printBoard(&board);

	while (strcmp (command,QUIT)!=0){ //program main loop

		fgets(inp,sizeof(inp),stdin);


		if (strlen(inp)>=sizeof(inp)-1){
			printf ("Error: command length must be less than or equal to 40 characters.\n");
			overflowflag=1;
		}

		else if (is_empty(inp) && overflowflag==1){
			overflowflag=0;
		}
		else{
			if (strlen(inp)==sizeof(inp)-1){
				overflowflag=1;
			}
			else{
				overflowflag=0;
			}
			sscanf (inp,"%s %d", command, &param);

			if (!strcmp (command,"set_number_steps") && param!=-1){
				if (gameover) printf ("Error: game is over.\n");
				else if (param>0 && param<8){
					num_steps=param;
					firstcommand=0;
				}
				else {
					printf ("Error: number of mini-max steps must be between 1-7.\n");
				}
			}
			else if (is_empty(inp)){
				printf ("Error: command must have at least one non-whitespace character.\n");
			}

			else if (!strcmp (command,"h")){

				printf("%d",getHeight(&board,param));
			}

			else if (!strcmp (command,"suggest_move")){

				FIRSTCOMMAND
				if (gameover) printf ("Error: game is over.\n");
				else{

				printf ("Suggested move: add disc to column [%d].\n",
					getBestChild(&board, num_steps, getChildren, free, evalFunc, 1).index+1);
			}}}
			else if (!strcmp (command,"add_disc")){

				FIRSTCOMMAND

				#ifdef DEBUG
					printf ("param: %d\n", param);
					printf ("height[%d]: %d\n",param-1, heights[param-1]);
				#endif
				if (param>7 || param<1)
					printf ("Error: column number must be in the range 1-7.\n");
				else if (heights[param-1]==6)
					printf ("Error: column [%d] is full.\n", param);
				else if (gameover) printf ("Error: game is over.\n");
				else if (heights[param-1]<6){
					insertDisc(heights[param-1], param-1, &board, 1);
					heights[param-1]++;
					if (evalFunc(&board)!=100000){
						compIndex= getBestChild(&board, num_steps, getChildren, free, evalFunc, 0).index;
						printf ("Computer move: add disc to column [%d].\n", compIndex+1 );
						insertDisc(heights[compIndex], compIndex, &board, -1);
						heights[compIndex]++;

					}
					printBoard(&board);
				}

			}}

			else if (!strcmp (command, "quit")){
				//just so the error msg won't be printed before leaving the while
			}

			else if (!strcmp (command, "restart_game")){

				board_init(&board); //restarting board
				for (i=0; i<6; i++){
					 heights[i]=0;
				} //restarting heights arr
				printf ("Game restarted.\n");
				gameover=0;
				printBoard(&board);
			}
			else{
				printf ("Error: command [%s] not supported.\n", command);

			}

			param=-1; //restarting the param
			//score checking and changing game over/not over value. 0: game on, else game over.
			score=evalFunc(&board);

			if (score==100000 && !gameover){

				printf  ("Game over: you win.\n");
				gameover=2;
			}
			else if (score==-100000 && !gameover){

				printf  ("Game over: computer wins.\n");
				gameover=2;
			}

			else if (isFull(board) && !gameover){
				printf ("Game over: board is full and there is no winner.\n");
				gameover=2;
			}
		}
	}
	//freeing main board on way out
	free_board(&board);
    return 0;
}


