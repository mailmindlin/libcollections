#include <stdbool.h>
#ifndef __LIBCOLLECTIONS_QUEUE_H
#define __LIBCOLLECTIONS_QUEUE_H

typedef int (Comparator)(void* a, void* b);
typedef void (Cleaner) (void* value);

typedef enum {
	UnknownCollectionType,
	FibonacciQueue,
	PairingQueue,
	BinaryQueue,
	KeyedFibonacciQueue,
	KeyedPairingQueue,
	BinaryHeap,
	NaryHeap,
	BinomialHeap,
	LinkedQueue,
	BrodalQueue,
	StrictFibonacciHeap
} CollectionType;

typedef struct {
	bool (*push) (RelativePriorityQueue* queue, void* value);
	void* (*peek) (RelativePriorityQueue* queue);
	void* (*pop) (RelativePriorityQueue* queue);
	bool (*isEmpty) (RelativePriorityQueue* queue);
	void (*clear) (RelativePriorityQueue* queue, Cleaner* cleaner);
	void (*release) (RelativePriorityQueue* queue, Cleaner* cleaner);
	CollectionType type;
	unsigned char[sizeof(void*) * 4] priv;//Implementation-specific data
} Queue;

typedef struct {
	bool (*push) (KeyedPriorityQueue* queue, unsigned int key, void* value);
	void* (*peek) (KeyedPriorityQueue* queue);
	unsigned int (*peekKey) (KeyedPriorityQueue* queue);
	void* (*pop) (KeyedPriorityQueue* queue);
	bool (*isEmpty) (KeyedPriorityQueue* queue);
	void (*clear) (KeyedPriorityQueue* queue, Cleaner* cleaner);
	void (*release) (KeyedPriorityQueue* queue, Cleaner* cleaner);
	CollectionType type;
	Comparator* comparator;
	unsigned char[sizeof(void*) * 4] priv;//Implementation-specific data
} KeyedPriorityQueue;

LIBCOLLECTIONS_PUBLIC RelativePriorityQueue* InitRelativePriorityQueue(RelativePriorityQueue* queue, RelativePriorityQueueType type, Comparator* comparator);

#endif
