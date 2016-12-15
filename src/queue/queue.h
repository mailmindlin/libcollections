#include <stdbool.h>
#ifndef __LIBCOLLECTIONS_QUEUE_H
#define __LIBCOLLECTIONS_QUEUE_H

typedef int (Comparator)(void* a, void* b);
typedef void (Cleaner) (void* value);

typedef enum {
	UnknownType,
	FibonacciHeap,
	PairingHeap,
	BinaryHeap,
	NaryHeap,
	BinomialHeap,
	LinkedQueue,
	BrodalQueue,
	StrictFibonacciHeap
} RelativePriorityQueueType;

typedef struct {
	bool (*push) (RelativePriorityQueue* queue, void* value);
	void* (*peek) (RelativePriorityQueue* queue);
	void* (*pop) (RelativePriorityQueue* queue);
	bool (*isEmpty) (RelativePriorityQueue* queue);
	void (*clear) (RelativePriorityQueue* queue, Cleaner* cleaner);
	void (*release) (RelativePriorityQueue* queue, Cleaner* cleaner);
	RelativePriorityQueueType type;
	Comparator* comparator;
	unsigned char[sizeof(void*) * 4] priv;//Implementation-specific data
} RelativePriorityQueue;

LIBCOLLECTIONS_PUBLIC RelativePriorityQueue* InitRelativePriorityQueue(RelativePriorityQueue* queue, RelativePriorityQueueType type, Comparator* comparator);

#endif