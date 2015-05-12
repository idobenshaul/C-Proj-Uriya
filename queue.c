#include "queue.h"
#include <stdlib.h>

// queue implementation

QueueRef queue_init(void* data) {
	return (QueueRef) newList(data);
}

void enqueue(QueueRef queue, void* data) {
	append((ListRef) queue, data);
}

void * peek(QueueRef queue) {
	return headData(queue);
}

QueueRef dequeue(QueueRef queue, FreeFunc freeData) {
	return (QueueRef) (pop(queue, freeData));
}
