#include "queue.h"
#include "../locality.h"

#ifndef __LIBCOLLECTIONS_QUEUE_PRIV_H
#define __LIBCOLLECTIONS_QUEUE_PRIV_H

typedef struct PairingRPQNode {
	struct PairingKPQNode* sibiling;
	struct PairingKPQNode* child;
	void* value;
} PairingRPQNode;

typedef struct PairingKPQNode {
	struct PairingKPQNode* sibiling;
	struct PairingKPQNode* child;
	unsigned int key;
	void* value;
} PairingKPQNode;

typedef struct FibonacciRPQNode {
	struct FibonacciRPQNode* parent;
	struct FibonacciRPQNode* left;
	struct FibonacciRPQNode* right;
	struct FibonacciRPQNode* child;
	void* value;
	size_t degree;
	bool marked;
} FibonacciRPQNode;

typedef struct FibonacciKPQNode {
	struct FibonacciKPQNode* parent;
	struct FibonacciKPQNode* left;
	struct FibonacciKPQNode* right;
	struct FibonacciKPQNode* child;
	unsigned int key;
	void* value;
	size_t degree;
	bool marked;
} FibonacciKPQNode;

union RelativePriorityQueueData {
	PairingRPQNode* pairingRoot;
	FibonacciRPQNode* fibonacciRoot;
};

union KeyedPriorityQueueData {
	PairingKPQNode* pairingRoot;
	FibonacciKPQNode* fibonacciRoot;
};

LIBCOLLECTIONS_LOCAL bool __RPQ_Priv0Null(RelativePriorityQueue* queue);
LIBCOLLECTIONS_LOCAL bool __KPQ_Priv0Null(KeyedPriorityQueue* queue);

//Fibonacci RPQ methods
LIBCOLLECTIONS_LOCAL bool  FibonacciRPQ_push(RelativePriorityQueue* queue, void* value);
LIBCOLLECTIONS_LOCAL void* FibonacciRPQ_peek(RelativePriorityQueue* queue);
LIBCOLLECTIONS_LOCAL void* FibonacciRPQ_pop (RelativePriorityQueue* queue);
LIBCOLLECTIONS_LOCAL void  FibonacciRPQ_clear(RelativePriorityQueue* queue, Cleaner* cleaner);
LIBCOLLECTIONS_LOCAL void  FibonacciRPQ_release(RelativePriorityQueue* queue, Cleaner* cleaner);

//Pairing heap RPQ methods
LIBCOLLECTIONS_LOCAL bool  PairingRPQ_push(RelativePriorityQueue* queue, void* value);
LIBCOLLECTIONS_LOCAL void* PairingRPQ_pop (RelativePriorityQueue* queue);
LIBCOLLECTIONS_LOCAL void* PairingRPQ_peek(RelativePriorityQueue* queue);
LIBCOLLECTIONS_LOCAL void  PairingRPQ_clear(RelativePriorityQueue* queue, Cleaner* cleaner);
LIBCOLLECTIONS_LOCAL void  PairingRPQ_release(RelativePriorityQueue* queue, Cleaner* cleaner);

//Fibonacci KPQ methods
LIBCOLLECTIONS_LOCAL bool  FibonacciKPQ_push(KeyedPriorityQueue* queue, unsigned int key, void* value);
LIBCOLLECTIONS_LOCAL void* FibonacciKPQ_peek(KeyedPriorityQueue* queue);
LIBCOLLECTIONS_LOCAL unsigned int FibonacciKPQ_peekKey(KeyedPriorityQueue* queue);
LIBCOLLECTIONS_LOCAL void* FibonacciKPQ_pop (KeyedPriorityQueue* queue);
LIBCOLLECTIONS_LOCAL void  FibonacciKPQ_clear(KeyedPriorityQueue* queue, Cleaner* cleaner);
LIBCOLLECTIONS_LOCAL void  FibonacciKPQ_release(KeyedPriorityQueue* queue, Cleaner* cleaner);

//Pairing heap KPQ methods
LIBCOLLECTIONS_LOCAL bool  PairingKPQ_push(KeyedPriorityQueue* queue, unsigned int key, void* value);
LIBCOLLECTIONS_LOCAL void* PairingKPQ_pop (KeyedPriorityQueue* queue);
LIBCOLLECTIONS_LOCAL void* PairingKPQ_peek(KeyedPriorityQueue* queue);
LIBCOLLECTIONS_LOCAL unsigned int PairingKPQ_peekKey(KeyedPriorityQueue* queue);
LIBCOLLECTIONS_LOCAL void  PairingKPQ_clear(KeyedPriorityQueue* queue, Cleaner* cleaner);
LIBCOLLECTIONS_LOCAL void  PairingKPQ_release(KeyedPriorityQueue* queue, Cleaner* cleaner);
#endif
