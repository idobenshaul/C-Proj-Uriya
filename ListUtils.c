#include <stdio.h>
#include <stdlib.h>
#include "ListUtils.h"

struct List{
	struct List* next;
	void* value;
};


ListRef newList(void* headData){
	ListRef new;
	new = malloc(sizeof(struct List));
	new->value = headData;
	new->next = NULL;

	return new;
}

void* headData(ListRef list){
	//printf ("headData");
	//printBoard((*Child)(list->value)->state);
	return (isEmpty(list))? NULL : list->value;

}

ListRef tail(ListRef list){

	if ((isEmpty(list)) || (list->next==NULL))
		return NULL;

	return list->next;
}

ListRef append(ListRef list, void* data){

	if (isEmpty(list)){
		list->value=data;
		return list;
	}
	else{
		while (tail(list)!=NULL){
			list=tail(list);
		}
		list->next=newList(data);
	}
	return tail(list);
}

void destroyList(ListRef list, FreeFunc freeData){
	ListRef curNext;
	int cnt=0;
	while (!isEmpty(list)){
		cnt++;
		freeData (list->value);
		curNext=tail(list);
		free(list);
		list=curNext;
	}
	if (cnt==0) printf ("cnt: %d", cnt);
	free(list);
}

int isEmpty(ListRef list){
	if (list==NULL)
		return 1;
	if (list->value==NULL && list->next==NULL)
		return 1;
	else {
		return 0;
	}
}

