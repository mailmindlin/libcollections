#include "queue.h"
#include "../locality.h"

#ifndef __LIBCOLLECTIONS_QUEUE_PRIV_H
#define __LIBCOLLECTIONS_QUEUE_PRIV_H

LIBCOLLECTIONS_LOCAL bool __RPQ_Priv0Null(RelativePriorityQueue* queue);
LIBCOLLECTIONS_LOCAL bool __KPQ_Priv0Null(KeyedPriorityQueue* queue);

//Fibonacci RPQ methods
LIBCOLLECTIONS_LOCAL bool  FibonacciRPQ_push(RelativePriorityQueue* queue, void* value);
LIBCOLLECTIONS_LOCAL void* FibonacciRPQ_peek(RelativePriorityQueue* queue);
LIBCOLLECTIONS_LOCAL void* FibonacciRPQ_pop (RelativePriorityQueue* queue);
LIBCOLLECTIONS_LOCAL bool  FibonacciRPQ_empty(RelativePriorityQueue* queue) __attribute__ ((weakref ('__RPQ_Priv0Null')));
LIBCOLLECTIONS_LOCAL void  FibonacciRPQ_clear(RelativePriorityQueue* queue);
LIBCOLLECTIONS_LOCAL void  FibonacciRPQ_release(RelativePriorityQueue* queue);

//Pairing heap RPQ methods
LIBCOLLECTIONS_LOCAL bool  PairingRPQ_push(RelativePriorityQueue* queue, void* value);
LIBCOLLECTIONS_LOCAL void* PairingRPQ_pop (RelativePriorityQueue* queue);
LIBCOLLECTIONS_LOCAL void* PairingRPQ_peek(RelativePriorityQueue* queue);
LIBCOLLECTIONS_LOCAL bool  PairingRPQ_empty(RelativePriorityQueue* queue) __attribute__ ((weakref ('__RPQ_Priv0Null')));
LIBCOLLECTIONS_LOCAL void  PairingRPQ_clear(RelativePriorityQueue* queue);
LIBCOLLECTIONS_LOCAL void  PairingRPQ_release(RelativePriorityQueue* queue);

//Fibonacci KPQ methods
LIBCOLLECTIONS_LOCAL bool  FibonacciKPQ_push(KeyedPriorityQueue* queue, unsigned int key, void* value);
LIBCOLLECTIONS_LOCAL void* FibonacciKPQ_peek(KeyedPriorityQueue* queue);
LIBCOLLECTIONS_LOCAL unsigned int FibonacciKPQ_peekKey(KeyedPriorityQueue* queue);
LIBCOLLECTIONS_LOCAL void* FibonacciKPQ_pop (KeyedPriorityQueue* queue);
LIBCOLLECTIONS_LOCAL bool  FibonacciKPQ_empty(KeyedPriorityQueue* queue) __attribute__ ((weakref ('__KPQ_Priv0Null')));
LIBCOLLECTIONS_LOCAL void  FibonacciKPQ_clear(KeyedPriorityQueue* queue);
LIBCOLLECTIONS_LOCAL void  FibonacciKPQ_release(KeyedPriorityQueue* queue);

//Pairing heap KPQ methods
LIBCOLLECTIONS_LOCAL bool  PairingKPQ_push(KeyedPriorityQueue* queue, unsigned int key, void* value);
LIBCOLLECTIONS_LOCAL void* PairingKPQ_pop (KeyedPriorityQueue* queue);
LIBCOLLECTIONS_LOCAL void* PairingKPQ_peek(KeyedPriorityQueue* queue);
LIBCOLLECTIONS_LOCAL unsigned int PairingKPQ_peekKey(KeyedPriorityQueue* queue);
LIBCOLLECTIONS_LOCAL bool  PairingKPQ_empty(KeyedPriorityQueue* queue) __attribute__ ((weakref ('__KPQ_Priv0Null')));
LIBCOLLECTIONS_LOCAL void  PairingKPQ_clear(KeyedPriorityQueue* queue);
LIBCOLLECTIONS_LOCAL void  PairingKPQ_release(KeyedPriorityQueue* queue);
#endif