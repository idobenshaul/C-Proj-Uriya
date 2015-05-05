

#include "ListUtils.h"
#ifndef QUEUE_H_
#define QUEUE_H_

typedef ListRef QueueRef;

QueueRef queue_init(void* data) ;
void enqueue(QueueRef queue, void* data);
void * peek(QueueRef queue);
QueueRef dequeue(QueueRef queue, FreeFunc freeData);

#endif /* QUEUE_H_ */
