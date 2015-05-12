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
void free_board(char ***board){
	int i=0;
	for (i=0; i< 7; i++){
		free((*board)[i]);
	}
	free (*board);
}
//frees the child passed to it along with it's fields
void childFree (void* child){
	if (child==NULL){
		//printf ("NULL \n\n");
		return;
	}
	if (sizeof(child)!=8){
		printf ("No Ideaa whatsoever\n\n");
		return;
	}

	Child* temp= (Child*) child;

	if (temp->state!=NULL){
		char** board=temp->state;

		free_board((char***)(&board));
	}
	free (temp);
}


//gets a list, and returns the max value out of the eval's of all it's members
void updateEvalChildren(ListRef list,int (*evaluate) (void* state)){
    ListRef cur=list;
    int cnt=0;
	Child* data;
	//struct MiniMaxResult res;
	printf("SO CLOSE\n");
	while(!isEmpty(cur)){
	    cnt++;
		data=(Child*)(headData(cur));
        printf("hey1");
		data->result.value=evaluate(&(data->state));
		printf("hey3");
		cur=tail(cur);
		printf("hey4");
	}
}
struct MiniMaxResult getListMaxEval(ListRef list,int (*evaluate) (void* state)){
	ListRef cur=list;
	int max=-199999, curEval;
	Child* data;

	struct MiniMaxResult res;
	while(!isEmpty(cur)){
		data=(Child*)(headData(cur));
		if((curEval=evaluate(&(data->state)))>max){
			max=curEval;
			res.index=(data->result).index;
			res.value=max;
		}
		cur=tail(cur);
	}
	return res;
}

//gets a list, and returns the min value out of the eval's of all it's members
struct MiniMaxResult getListMinEval(ListRef list,int (*evaluate) (void* state)){
	ListRef cur=list;
	int min=199999, curEval;
	Child* data;

	struct MiniMaxResult res;
	while(!isEmpty(cur)){
		data=(Child*)(headData(cur));
		//(data->result).value=evalFunc(data->state);
        if((curEval=evaluate(&(data->state)))<min){
			min=curEval;
			res.index=(data->result).index;
			res.value=min;
		}
		cur=tail(cur);
	}
	return res;
}
//according to the eval func, and the min max algo, returns the most favored child for the inputs.
/*char callMiniMax(void* state,
		unsigned int maxDepth,
		ListRef (*getChildren) (void* state),
		FreeFunc freeState,
		int (*evaluate) (void* state),
		int isMaxPlayer, int actualCatRow,int actualCatCol,int actualMouseRow,int actualMouseCol
*/
struct MiniMaxResult getBestChild(
		void* state,
		unsigned int maxDepth,
		ListRef (*getChildren) (void* state),
		FreeFunc freeState,
		int (*evaluate) (void* state),
		int isMaxPlayer){



        struct MiniMaxResult res;
        res=alphabeta(state,maxDepth,-199999,199999,getChildren,freeState,evaluate,isMaxPlayer);
        return res;
    }

struct MiniMaxResult alphabeta(void* state, unsigned int maxDepth, int alpha, int beta,
        ListRef (*getChildren) (void* state),FreeFunc freeState,
        int (*evaluate) (void* state), int isMaxPlayer){

        char*** board =(char***) state;
        struct MiniMaxResult result= {-100,-100};
        ListRef children, cur;
        //creating a list of children and filling it with scores
        /*
        if (maxDepth==1){ //recursion base

            //Child->result=evalFunc(Child->state);
            //updateEvalChildren(children,evaluate);
            if (isMaxPlayer){
                result= getListMaxEval(children,evaluate);
            }
            else{
                result= getListMinEval(children,evaluate);
            }

        }*/
        if(maxDepth==0){
            printf("FUCkFuckFuck\n");
            //printBoard(board);
            struct MiniMaxResult res;
            res.value=evaluate(state);
            return res;
        }

        else{ //recursion step

            children = getChildren(state);
            cur=children;
            struct MiniMaxResult root;
            if (isMaxPlayer){
                //struct MiniMaxResult root;
                root.value=-199999;
                switchIsOriginalTurn();
                switchTurn();
                //ListRef levelKids=newList(NULL);
                struct MiniMaxResult alphabBetaResult;

                while (!isEmpty(cur)){
                //recursive call with depth-1, start state is the state of this node, opposite of max player
                    Child* data=(Child*)(headData(cur));
                    Child* newChild = (Child*)malloc(sizeof(struct Child));
                    alphabBetaResult=alphabeta(&(data->state),maxDepth-1,alpha,beta,getChildren,freeState,evaluate,!isMaxPlayer);
                    //newChild->result.value=alphabBetaResult.value;
                    //newChild->result.index=alphabBetaResult.index;
                    //newChild->state=NULL;
                    if(root.value < alphabBetaResult.value){
                        root.value=alphabBetaResult.value;
                        root.index=data->result.index;
                    }
                    if(alpha<root.value){
                        alpha=root.value;
                    }
                    if(alpha>=beta){
                        break;
                    }
                    //append(levelKids, newChild);
                    cur=tail(cur);

                }
            return root;
            }

            else{
                root.value=199999;
                switchIsOriginalTurn();
                switchTurn();
                //ListRef levelKids=newList(NULL);
                struct MiniMaxResult alphabBetaResult;

                while (!isEmpty(cur)){
                //recursive call with depth-1, start state is the state of this node, opposite of max player
                    Child* data=(Child*)(headData(cur));
                    Child* newChild = (Child*)malloc(sizeof(struct Child));
                    alphabBetaResult=alphabeta(&(data->state),maxDepth-1,alpha,beta,getChildren,freeState,evaluate,!isMaxPlayer);
                    //newChild->result.value=alphabBetaResult.value;
                    //newChild->result.index=alphabBetaResult.index;
                    //newChild->state=NULL;
                    if(root.value> alphabBetaResult.value){
                        root.value=alphabBetaResult.value;
                        root.index=alphabBetaResult.index;
                    }
                    if(beta>root.value){
                        beta=root.value;
                    }
                    if(alpha>=beta){
                        break;
                    }
                    //append(levelKids, newChild);
                    cur=tail(cur);
                }
            return root;
            }




        }
        //destroyList(children, childFree);
        //return result;
}




// check for max value of children
