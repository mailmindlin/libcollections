#include <stdbool.h>
#include "../collections.h"
#include "../locality.h"
#ifndef __LIBCOLLECTIONS_QUEUE_H
#define __LIBCOLLECTIONS_QUEUE_H

typedef int (Comparator)(void* a, void* b);
typedef void (Cleaner) (void* value);

struct Queue_s;
struct RelativePriorityQueue_s;
struct KeyedPriorityQueue_s;

LIBCOLLECTIONS_PUBLIC typedef struct Queue_s Queue;
LIBCOLLECTIONS_PUBLIC typedef struct RelativePriorityQueue_s RelativePriorityQueue;
LIBCOLLECTIONS_PUBLIC typedef struct KeyedPriorityQueue_s KeyedPriorityQueue;

struct Queue_s {
	bool (*push) (Queue* queue, void* value);
	void* (*peek) (Queue* queue);
	void* (*pop) (Queue* queue);
	bool (*isEmpty) (Queue* queue);
	void (*clear) (Queue* queue, Cleaner* cleaner);
	void (*release) (Queue* queue, Cleaner* cleaner);
	CollectionType type;
	void* priv[sizeof(void*) * 4];//Implementation-specific data
};

struct RelativePriorityQueue_s {
	bool (*push) (RelativePriorityQueue* queue, void* value);
	void* (*peek) (RelativePriorityQueue* queue);
	void* (*pop) (RelativePriorityQueue* queue);
	bool (*isEmpty) (RelativePriorityQueue* queue);
	void (*clear) (RelativePriorityQueue* queue, Cleaner* cleaner);
	void (*release) (RelativePriorityQueue* queue, Cleaner* cleaner);
	CollectionType type;
	Comparator* comparator;
	void* priv[sizeof(void*) * 4];//Implementation-specific data
};

struct KeyedPriorityQueue_s {
	bool (*push) (KeyedPriorityQueue* queue, unsigned int key, void* value);
	void* (*peek) (KeyedPriorityQueue* queue);
	unsigned int (*peekKey) (KeyedPriorityQueue* queue);
	void* (*pop) (KeyedPriorityQueue* queue);
	bool (*isEmpty) (KeyedPriorityQueue* queue);
	void (*clear) (KeyedPriorityQueue* queue, Cleaner* cleaner);
	void (*release) (KeyedPriorityQueue* queue, Cleaner* cleaner);
	CollectionType type;
	void* priv[sizeof(void*) * 4];//Implementation-specific data
};

Queue* LIBCOLLECTIONS_PUBLIC InitQueue(Queue* queue, CollectionType type);
RelativePriorityQueue* LIBCOLLECTIONS_PUBLIC InitRelativePriorityQueue(RelativePriorityQueue* queue, CollectionType type, Comparator* comparator);
KeyedPriorityQueue* LIBCOLLECTIONS_PUBLIC InitKeyedPriorityQueue(KeyedPriorityQueue* queue, CollectionType type);

#endif
