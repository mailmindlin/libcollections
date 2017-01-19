#include "queue.h"
#include "../locality.h"

#ifndef __LIBCOLLECTIONS_QUEUE_PRIV_H
#define __LIBCOLLECTIONS_QUEUE_PRIV_H

typedef struct PairingRPQNode {
	struct PairingRPQNode* sibiling;
	struct PairingRPQNode* child;
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

//Linked Queue methods
LIBCOLLECTIONS_LOCAL bool  LinkedFIFO_push(Queue* queue, void* value);
LIBCOLLECTIONS_LOCAL void* LinkedFIFO_peek(Queue* queue);
LIBCOLLECTIONS_LOCAL void* LinkedFIFO_pop (Queue* queue);
LIBCOLLECTIONS_LOCAL bool  LinkedFIFO_empty(Queue* queue);
LIBCOLLECTIONS_LOCAL void  LinkedFIFO_clear(Queue* queue, Consumer* Consumer);
LIBCOLLECTIONS_LOCAL void  LinkedFIFO_release(Queue* queue, Consumer* Consumer);

//Linked LIFO methods
LIBCOLLECTIONS_LOCAL bool  LinkedLIFO_push(Queue* queue, void* value);
LIBCOLLECTIONS_LOCAL void* LinkedLIFO_peek(Queue* queue);
LIBCOLLECTIONS_LOCAL void* LinkedLIFO_pop (Queue* queue);
LIBCOLLECTIONS_LOCAL bool  LinkedLIFO_empty(Queue* queue);
LIBCOLLECTIONS_LOCAL void  LinkedLIFO_clear(Queue* queue, Consumer* Consumer);
LIBCOLLECTIONS_LOCAL void  LinkedLIFO_release(Queue* queue, Consumer* Consumer);

//Fibonacci RPQ methods
LIBCOLLECTIONS_LOCAL bool  FibonacciRPQ_push(Queue* queue, void* value);
LIBCOLLECTIONS_LOCAL void* FibonacciRPQ_peek(Queue* queue);
LIBCOLLECTIONS_LOCAL void* FibonacciRPQ_pop (Queue* queue);
LIBCOLLECTIONS_LOCAL bool  FibonacciRPQ_empty(Queue* queue);
LIBCOLLECTIONS_LOCAL void  FibonacciRPQ_clear(Queue* queue, Consumer* Consumer);
LIBCOLLECTIONS_LOCAL void  FibonacciRPQ_release(Queue* queue, Consumer* Consumer);

//Pairing heap RPQ methods
LIBCOLLECTIONS_LOCAL bool  PairingRPQ_push(Queue* queue, void* value);
LIBCOLLECTIONS_LOCAL void* PairingRPQ_pop (Queue* queue);
LIBCOLLECTIONS_LOCAL void* PairingRPQ_peek(Queue* queue);
LIBCOLLECTIONS_LOCAL bool  PairingRPQ_empty(Queue* queue);
LIBCOLLECTIONS_LOCAL void  PairingRPQ_clear(Queue* queue, Consumer* Consumer);
LIBCOLLECTIONS_LOCAL void  PairingRPQ_release(Queue* queue, Consumer* Consumer);

//Fibonacci KPQ methods
LIBCOLLECTIONS_LOCAL bool  FibonacciKPQ_push(KeyedPriorityQueue* queue, unsigned int key, void* value);
LIBCOLLECTIONS_LOCAL void* FibonacciKPQ_peek(KeyedPriorityQueue* queue);
LIBCOLLECTIONS_LOCAL unsigned int FibonacciKPQ_peekKey(KeyedPriorityQueue* queue);
LIBCOLLECTIONS_LOCAL void* FibonacciKPQ_pop (KeyedPriorityQueue* queue);
LIBCOLLECTIONS_LOCAL bool  FibonacciKPQ_empty(KeyedPriorityQueue* queue);
LIBCOLLECTIONS_LOCAL void  FibonacciKPQ_clear(KeyedPriorityQueue* queue, Consumer* Consumer);
LIBCOLLECTIONS_LOCAL void  FibonacciKPQ_release(KeyedPriorityQueue* queue, Consumer* Consumer);

//Pairing heap KPQ methods
LIBCOLLECTIONS_LOCAL bool  PairingKPQ_push(KeyedPriorityQueue* queue, unsigned int key, void* value);
LIBCOLLECTIONS_LOCAL void* PairingKPQ_pop (KeyedPriorityQueue* queue);
LIBCOLLECTIONS_LOCAL void* PairingKPQ_peek(KeyedPriorityQueue* queue);
LIBCOLLECTIONS_LOCAL unsigned int PairingKPQ_peekKey(KeyedPriorityQueue* queue);
LIBCOLLECTIONS_LOCAL bool  PairingKPQ_empty(KeyedPriorityQueue* queue);
LIBCOLLECTIONS_LOCAL void  PairingKPQ_clear(KeyedPriorityQueue* queue, Consumer* Consumer);
LIBCOLLECTIONS_LOCAL void  PairingKPQ_release(KeyedPriorityQueue* queue, Consumer* Consumer);

#endif
