#include <stdio.h>
#include <stdlib.h>
#include "MiniMax.h"


/**
 * Returns the best child of the given state, resulting from the MiniMax computation up to maxDepth.
 * The getChildren function will be used to get each state's children.
 * The freeState function will be used to deallocate any lists returned by GetChildren.
 * The evaluate function will be used to evaluate the state score.
 * The player who's turn it is to play at the given state is assumed to be the MAX player if and only if isMaxPlayer is non-zero,
 * otherwise, it is assumed to be the MIN player.
 *
 * Will NOT crash in case one of the given functions fails but result will be undefined.
 */

//frees the current cells in the board, and the board itself
void free_board(int ***board){
	int i=0;
	for (i=0; i< 6; i++){
		free((*board)[i]);
	}
	free (*board);
}
//frees the child passed to it along with it's fields
void childFree (void* child){
	if (child==NULL){
		printf ("NULL \n\n");
		return;
	}
	if (sizeof(child)!=8){
		printf ("NULL \n\n");
		return;
	}

	Child* temp= (Child*) child;

	if (temp->state!=NULL){
		int** board=temp->state;

		free_board((int***)(&board));
	}
	free (temp);
}


//gets a list, and returns the max value out of the eval's of all it's members
struct MiniMaxResult getListMaxEval(ListRef list){
	ListRef cur=list;
	int max=-199999, curEval;
	Child* data;

	struct MiniMaxResult res;
	while(!isEmpty(cur)){
		data=(Child*)(headData(cur));
		if((curEval=(data->result).value)>max){
			max=curEval;
			res.index=(data->result).index;
			res.value=max;
		}
		cur=tail(cur);
	}
	return res;
}
//gets a list, and returns the min value out of the eval's of all it's members
struct MiniMaxResult getListMinEval(ListRef list){
	ListRef cur=list;
	int min=199999, curEval;
	Child* data;

	struct MiniMaxResult res;
	while(!isEmpty(cur)){
		data=(Child*)(headData(cur));
		if((curEval=(data->result).value)<min){
			min=curEval;
			res.index=(data->result).index;
			res.value=min;
		}
		cur=tail(cur);
	}
	return res;
}
//according to the eval func, and the min max algo, returns the most favored child for the inputs.
struct MiniMaxResult getBestChild(
		void* state,
		unsigned int maxDepth,
		ListRef (*getChildren) (void* state),
		FreeFunc freeState,
		int (*evaluate) (void* state),
		int isMaxPlayer){

	struct MiniMaxResult result= {-100,-100};
	ListRef children, cur;

	children = getChildren(state);

	cur=children;

	//creating a list of children and filling it with scores

	if (maxDepth==1){ //recursion base
		if (isMaxPlayer){
			result= getListMaxEval(children);
		}
		else{
			result= getListMinEval(children);
		}

	}
	else{ //recursion step
		ListRef levelKids=newList(NULL);
		while (!isEmpty(cur)){
		//recursive call with depth-1, start state is the state of this node, opposite of max player
			Child* data=(Child*)(headData(cur));
			Child* newChild = (Child*)malloc(sizeof(struct Child));
			newChild->result.value=getBestChild(&(data->state),maxDepth-1,
							getChildren,freeState,evaluate,	!isMaxPlayer).value;
			newChild->result.index=data->result.index;
			newChild->state=NULL;
			append(levelKids, newChild);
			cur=tail(cur);

		}
		if (isMaxPlayer){
			result= getListMaxEval(levelKids);
		}
		else{
			result= getListMinEval(levelKids);
		}
		destroyList(levelKids, childFree);


	}
	destroyList(children, childFree);
	return result;
}



// check for max value of children



