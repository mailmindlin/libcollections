#include <stdbool.h>
#include "../collections.h"
#ifndef __LIBCOLLECTIONS_QUEUE_H
#define __LIBCOLLECTIONS_QUEUE_H

typedef int (Comparator)(void* a, void* b);
typedef void (Cleaner) (void* value);

struct Queue_s;
struct RelativePriorityQueue_s;
struct KeyedPriorityQueue_s;

typedef struct Queue_s Queue;
typedef struct RelativePriorityQueue_s RelativePriorityQueue;
typedef struct KeyedPriorityQueue_s KeyedPriorityQueue;

struct Queue_s {
	bool (*push) (Queue* queue, void* value);
	void* (*peek) (Queue* queue);
	void* (*pop) (Queue* queue);
	bool (*isEmpty) (Queue* queue);
	void (*clear) (Queue* queue, Cleaner* cleaner);
	void (*release) (Queue* queue, Cleaner* cleaner);
	CollectionType type;
	unsigned char[sizeof(void*) * 4] priv;//Implementation-specific data
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
	unsigned char[sizeof(void*) * 4] priv;//Implementation-specific data
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
	unsigned char[sizeof(void*) * 4] priv;//Implementation-specific data
};

LIBCOLLECTIONS_PUBLIC Queue* InitQueue(Queue* queue, CollectionType type);
LIBCOLLECTIONS_PUBLIC RelativePriorityQueue* InitRelativePriorityQueue(RelativePriorityQueue* queue, CollectionType type, Comparator* comparator);
LIBCOLLECTIONS_PUBLIC KeyedPriorityQueue* InitKeyedPriorityQueue(KeyedPriorityQueue* queue, CollectionType type);

#endif
