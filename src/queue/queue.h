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

struct PairingRPQNode;
struct PairingKPQNode;
struct FibonacciRPQNode;
struct FibonacciKPQNode;
struct SinglyLinkedNode;

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
	//Implementation-specific data. Please don't touch.
	union {
		union {
			struct {
				struct SinglyLinkedNode* head;
			} linkedFIFOData;
		} fifo;
		union {
			struct {
				struct SinglyLinkedNode* head;
				struct SinglyLinkedNode* tail;
			} linkedLIFOData;
		} lifo;
		struct {
			Comparator* comparator;
			union {
				struct {
					struct PairingRPQNode* root;
				} pairingRPQData;
				struct {
					struct FibonacciRPQNode* root;
				} fibonacciRPQData;
			};
		} rpq;
	};
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
	//Implementation-specific data. Please don't touch.
	union {
		struct {
			struct PairingKPQNode* root;
		} pairingKPQData;
		struct {
			struct FibonacciKPQNode* root;
		} fibonacciKPQData;
	};
};

Queue* LIBCOLLECTIONS_PUBLIC InitQueue(Queue* queue, CollectionType type);
Queue* LIBCOLLECTIONS_PUBLIC InitRelativePriorityQueue(Queue* queue, CollectionType type, Comparator* comparator);
KeyedPriorityQueue* LIBCOLLECTIONS_PUBLIC InitKeyedPriorityQueue(KeyedPriorityQueue* queue, CollectionType type);

#endif
