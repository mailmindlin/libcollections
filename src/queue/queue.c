#include "queue.h"
#include "queue-priv.h"

typedef struct {
	bool (*push) (Queue* queue, void* value);
	void* (*peek) (Queue* queue);
	void* (*pop) (Queue* queue);
	bool (*isEmpty) (Queue* queue);
	void (*clear) (Queue* queue, Cleaner* cleaner);
	void (*release) (Queue* queue, Cleaner* cleaner);
} RelativePriorityQueuePrototype;
const RelativePriorityQueuePrototype[] = {
	[UnknownType] = {0},
	[FibonacciHeap] = {
		.push = 
	}
}
static bool _Priv0Null(RelativePriorityQueue* queue) {
	return queue->priv[0] == NULL;
}
static bool _InitClearPriv(RelativePriorityQueue* queue) {
	memset(&queue->priv, 0x00, 4);
	return true;
}
RelativePriorityQueue* InitRelativePriorityQueue(RelativePriorityQueue* queue, RelativePriorityQueueType type, Comparator* comparator) {
	queue->comparator = comparator;
	queue->type = type;
	switch (type) {
		case UnknownType:
			return NULL;
		case FibonacciHeap:
		case 
	}
}