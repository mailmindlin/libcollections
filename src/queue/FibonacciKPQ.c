#include <stddef.h>	//For NULL
#include <stdlib.h>	//For malloc, free
#include <stdbool.h>
#include "queue.h"
#include "queue-priv.h"

LIBCOLLECTIONS_LOCAL bool  FibonacciKPQ_push(KeyedPriorityQueue* queue, unsigned int key, void* value) {
	return false;
}

LIBCOLLECTIONS_LOCAL void* FibonacciKPQ_peek(KeyedPriorityQueue* queue) {
	return NULL;
}

LIBCOLLECTIONS_LOCAL unsigned int FibonacciKPQ_peekKey(KeyedPriorityQueue* queue) {
	return 0;
}

LIBCOLLECTIONS_LOCAL void* FibonacciKPQ_pop (KeyedPriorityQueue* queue) {
	return NULL;
}

LIBCOLLECTIONS_LOCAL bool  FibonacciKPQ_empty(KeyedPriorityQueue* queue) {
	return queue->fibonacciKPQData.root == NULL;
}

LIBCOLLECTIONS_LOCAL void  FibonacciKPQ_clear(KeyedPriorityQueue* queue, Consumer* cleaner) {
	
}

LIBCOLLECTIONS_LOCAL void  FibonacciKPQ_release(KeyedPriorityQueue* queue, Consumer* cleaner) {
	
}