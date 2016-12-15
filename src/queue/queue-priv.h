#include "queue.h"
#include "../locality.h"

#ifndef __LIBCOLLECTIONS_QUEUE_PRIV_H
#define __LIBCOLLECTIONS_QUEUE_PRIV_H

//Fibonacci RPQ methods
LIBCOLLECTIONS_LOCAL bool  FibonacciRPQ_init(RelativePriorityQueue* queue);
LIBCOLLECTIONS_LOCAL bool  FibonacciRPQ_push(RelativePriorityQueue* queue, void* value);
LIBCOLLECTIONS_LOCAL void* FibonacciRPQ_peek(RelativePriorityQueue* queue);
LIBCOLLECTIONS_LOCAL void* FibonacciRPQ_pop (RelativePriorityQueue* queue);
LIBCOLLECTIONS_LOCAL bool  FibonacciRPQ_empty(RelativePriorityQueue* queue);
LIBCOLLECTIONS_LOCAL void  FibonacciRPQ_clear(RelativePriorityQueue* queue);
LIBCOLLECTIONS_LOCAL void  FibonacciRPQ_release(RelativePriorityQueue* queue);

//Pairing heap RPQ methods
LIBCOLLECTIONS_LOCAL bool  PairingRPQ_init(RelativePriorityQueue* queue);
LIBCOLLECTIONS_LOCAL bool  PairingRPQ_push(RelativePriorityQueue* queue, void* value);
LIBCOLLECTIONS_LOCAL void* PairingRPQ_pop (RelativePriorityQueue* queue);
LIBCOLLECTIONS_LOCAL void* PairingRPQ_peek(RelativePriorityQueue* queue);
LIBCOLLECTIONS_LOCAL bool  PairingRPQ_empty(RelativePriorityQueue* queue);
LIBCOLLECTIONS_LOCAL void  PairingRPQ_clear(RelativePriorityQueue* queue);
LIBCOLLECTIONS_LOCAL void  PairingRPQ_release(RelativePriorityQueue* queue);
#endif