#include <stddef.h>	//For NULL
#include <string.h>	//For memset
#include "queue.h"
#include "queue-priv.h"

KeyedPriorityQueue* InitKeyedPriorityQueue(KeyedPriorityQueue* queue, CollectionType type) {
	queue->type = type;
	switch (type) {
		case FibonacciKPQ:
			queue->fibonacciKPQData.root = NULL;
			queue->push   = &FibonacciKPQ_push;
			queue->peek   = &FibonacciKPQ_peek;
			queue->pop    = &FibonacciKPQ_pop;
			queue->isEmpty= &FibonacciKPQ_empty;
			queue->clear  = &FibonacciKPQ_clear;
			queue->release= &FibonacciKPQ_release;
			break;
		case PairingKPQ:
			queue->pairingKPQData.root = NULL;
			queue->push   = &PairingKPQ_push;
			queue->peek   = &PairingKPQ_peek;
			queue->pop    = &PairingKPQ_pop;
			queue->isEmpty= &PairingKPQ_empty;
			queue->clear  = &PairingKPQ_clear;
			queue->release= &PairingKPQ_release;
			break;
		default:
			return NULL;
	}
	return queue;
}

Queue* LIBCOLLECTIONS_PUBLIC InitQueue(Queue* queue, CollectionType type) {
	queue->type = type;
	switch (type) {
		case UnknownCollectionType:
		case LinkedFIFO:
			queue->fifo.linkedFIFOData.head = NULL;
			queue->push   = &LinkedFIFO_push;
			queue->peek   = &LinkedFIFO_peek;
			queue->pop    = &LinkedFIFO_pop;
			queue->isEmpty= &LinkedFIFO_empty;
			queue->clear  = &LinkedFIFO_clear;
			queue->release= &LinkedFIFO_release;
			break;
		case LinkedLIFO:
			queue->lifo.linkedLIFOData.head = NULL;
			queue->push   = &LinkedLIFO_push;
			queue->peek   = &LinkedLIFO_peek;
			queue->pop    = &LinkedLIFO_pop;
			queue->isEmpty= &LinkedLIFO_empty;
			queue->clear  = &LinkedLIFO_clear;
			queue->release= &LinkedLIFO_release;
			break;
		default:
			return NULL;
	}
	return queue;
}

Queue* InitRelativePriorityQueue(Queue* queue, CollectionType type, Comparator* comparator) {
	queue->type = type;
	queue->rpq.comparator = comparator;
	switch (type) {
		case FibonacciRPQ:
			queue->rpq.fibonacciRPQData.root = NULL;
			queue->push   = &FibonacciRPQ_push;
			queue->peek   = &FibonacciRPQ_peek;
			queue->pop    = &FibonacciRPQ_pop;
			queue->isEmpty= &FibonacciRPQ_empty;
			queue->clear  = &FibonacciRPQ_clear;
			queue->release= &FibonacciRPQ_release;
			break;
		case UnknownCollectionType:
		case PairingRPQ:
			queue->rpq.pairingRPQData.root = NULL;
			queue->push   = &PairingRPQ_push;
			queue->peek   = &PairingRPQ_peek;
			queue->pop    = &PairingRPQ_pop;
			queue->isEmpty= &PairingRPQ_empty;
			queue->clear  = &PairingRPQ_clear;
			queue->release= &PairingRPQ_release;
			break;
		default:
			return NULL;
	}
	return queue;
}
