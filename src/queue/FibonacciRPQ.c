#include <stddef.h>	//For NULL
#include <stdlib.h>	//For malloc, free
#include <stdbool.h>
#include "queue.h"
#include "queue-priv.h"

LIBCOLLECTIONS_LOCAL bool  FibonacciRPQ_push(Queue* queue, void* value) {
	return false;
}

LIBCOLLECTIONS_LOCAL void* FibonacciRPQ_peek(Queue* queue) {
	return NULL;
}

LIBCOLLECTIONS_LOCAL void* FibonacciRPQ_pop (Queue* queue) {
	return NULL;
}

LIBCOLLECTIONS_LOCAL bool  FibonacciRPQ_empty(Queue* queue) {
	return queue->rpq.fibonacciRPQData.root == NULL;
}

LIBCOLLECTIONS_LOCAL void  FibonacciRPQ_clear(Queue* queue, Cleaner* cleaner) {
	
}

LIBCOLLECTIONS_LOCAL void  FibonacciRPQ_release(Queue* queue, Cleaner* cleaner) {
	
}