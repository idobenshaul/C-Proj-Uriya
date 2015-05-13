/*
 * BFS.c
 *
 *  Created on: May 5, 2015
 *      Author: uriyabartal
 */
#include "queue.h"
#include "logic.h"
#include <stdlib.h>
#include <stdio.h>

int findShortestPath(char*** board, int srcRow, int srcCol, int destRow,
		int destCol);

struct Slot {
	int row, col;
};
typedef struct Slot* SlotRef;


/*
int main() {
	char** board = loadGame(3);
	updateBoardStatus(board);

	printf("%d", findShortestPath(&board, mouseRow, mouseCol, cheeseRow+1, cheeseCol));

	return 0;
}*/


int main() {
	char** board = loadGame(3);
	updateBoardStatus(board);

	printf("%d", findShortestPath(&board, mouseRow, mouseCol, cheeseRow+1, cheeseCol));

	return 0;
}


int findShortestPath(char*** board, int srcRow, int srcCol, int destRow,
		int destCol) {
	int direction, cnt = 0, i, j;
	int prevCol, prevRow, newCol, newRow;

	for (i=0; i<7; i++){
		for (j=0;j<7;j++){
			visited[i][j]=0;
			dist[i][j]=0;
		}
	}

	/*if (!checkCoordinates(x, y) || !checkCoordinates(srcRow, srcCol)) {
	 puts("BFS failure- wrong input");
	 }*/
    printBoard(*board);
	//inits a matrix holding for each space if it has been visited=1 or not=0
	SlotRef slot = malloc(sizeof(struct Slot));

	slot->row = srcRow;
	slot->col = srcCol;
	QueueRef q = queue_init(slot);
	visited[srcRow][srcCol] = 1;

	while (!isEmpty(q)) {


		slot = peek(q);

		for (direction = 1; direction < 5; direction++) {
			if (isLegalMove(board, slot->row, slot->col, direction)) {
				//save previous location
				prevCol = newCol =slot->col;
				prevRow = newRow= slot->row;

				//update indices to new location
				switch (direction) {
				case LEFT:
					newCol=(slot->col)-1;
					break;
				case RIGHT:
					newCol=(slot->col)+1;
					break;
				case DOWN:
					newRow=(slot->row)+1;
					break;
				case UP:
					newRow=(slot->row)-1;
					break;
				}
				if (!visited[newRow][newCol]) {
					SlotRef temp = malloc(sizeof(struct Slot));
					temp->row = newRow;
					temp->col = newCol;
					enqueue(q, temp);
					//printSlotList((ListRef)q);
					visited[newRow][newCol] = 1;
					dist[newRow][newCol] = dist[prevRow][prevCol] + 1;
<<<<<<< HEAD
					//printVisited();
					//printf("Now I am waiting for input:\n");
					//scanf("");
					//if (newRow==destRow && newCol==destCol){
					//	return dist[newRow][newCol];
					//}
=======
					printVisited();
					if (newRow==destRow && newCol==destCol){
						return dist[newRow][newCol];
					}
>>>>>>> ae0cc88eeaa32c526beff2b7fe7c75bd944b2cdb

				}

			} //end for

		}
		cnt++;
		//printf(
		//		"%d: src X: %d src Y: %d slot X: %d slot Y: %d prev X: %d prev Y: %d, dist: %d \n",
		//		cnt, srcRow, srcCol, slot->row, slot->col, prevRow, prevCol,
		//		dist[destRow][destCol]);
		//printVisited();
		q = dequeue(q, free);
	} //end while

	puts ("BFS Finished");
<<<<<<< HEAD
=======

	//destination is unreachable
	return 1000;
}
>>>>>>> ae0cc88eeaa32c526beff2b7fe7c75bd944b2cdb

	//destination is unreachable
	return 1000;
}

void printVisited(){
	int i,j;
	puts("");
	for (i = 0; i < 7; i++) {
				for (j = 0; j < 7; j++) {
					printf("%d\t", dist[i][j]);
				}
				puts("");
	}
}

void printSlotList(ListRef l){

	while (!isEmpty(l)){
		SlotRef temp=headData(l);
		printf ("(%d,%d)", temp->row, temp->col);
		l=tail(l);
	}
}
