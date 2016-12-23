#include <stddef.h>	//For NULL
#include <stdlib.h>	//For malloc, free
#include <stdbool.h>
#include "queue.h"
#include "queue-priv.h"

LIBCOLLECTIONS_LOCAL bool  FibonacciRPQ_push(RelativePriorityQueue* queue, void* value) {
	return false;
}

LIBCOLLECTIONS_LOCAL void* FibonacciRPQ_peek(RelativePriorityQueue* queue) {
	return NULL;
}

LIBCOLLECTIONS_LOCAL void* FibonacciRPQ_pop (RelativePriorityQueue* queue) {
	return NULL;
}

LIBCOLLECTIONS_LOCAL bool  FibonacciRPQ_empty(RelativePriorityQueue* queue) {
	return queue->fibonacciRPQData.root == NULL;
}

LIBCOLLECTIONS_LOCAL void  FibonacciRPQ_clear(RelativePriorityQueue* queue, Cleaner* cleaner) {
	
}

LIBCOLLECTIONS_LOCAL void  FibonacciRPQ_release(RelativePriorityQueue* queue, Cleaner* cleaner) {
	
}