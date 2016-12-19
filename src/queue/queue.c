#include <stddef.h>	//For NULL
#include <string.h>	//For memset
#include "queue.h"
#include "queue-priv.h"

bool __RPQ_Priv0Null(RelativePriorityQueue* queue) {
	return queue->priv[0] == NULL;
}

bool __KPQ_Priv0Null(KeyedPriorityQueue* queue) {
	return queue->priv[0] == NULL;
}

KeyedPriorityQueue* InitKeyedPriorityQueue(KeyedPriorityQueue* queue, CollectionType type) {
	queue->type = type;
	memset(&queue->priv, 0x00, sizeof(queue->priv));
	switch (type) {
		case KeyedFibonacciPriorityQueue:
			queue->push   = &FibonacciKPQ_push;
			queue->peek   = &FibonacciKPQ_peek;
			queue->pop    = &FibinacciKPQ_pop;
			queue->isEmpty= &__KPQ_Priv0Null;
			queue->clear  = &FibonacciKPQ_clear;
			queue->release= &FibonacciKPQ_release;
			break;
		case KeyedPairingPriorityQueue:
			queue->push   = &PairingKPQ_push;
			queue->peek   = &PairingKPQ_peek;
			queue->pop    = &PairingKPQ_pop;
			queue->isEmpty= &__KPQ_Priv0Null;
			queue->clear  = &PairingKPQ_clear;
			queue->release= &PairingKPQ_release;
			break;
		default:
			return NULL;
	}
	return queue;
}

RelativePriorityQueue* InitRelativePriorityQueue(RelativePriorityQueue* queue, CollectionType type, Comparator* comparator) {
	queue->comparator = comparator;
	queue->type = type;
	memset(&queue->priv, 0x00, sizeof(queue->priv));
	switch (type) {
		case RelativeFibonacciPriorityQueue:
			queue->push   = &FibonacciRPQ_push;
			queue->peek   = &FibonacciRPQ_peek;
			queue->pop    = &FibinacciRPQ_pop;
			queue->isEmpty= &__RPQ_Priv0Null;
			queue->clear  = &FibonacciRPQ_clear;
			queue->release= &FibonacciRPQ_release;
			break;
		case RelativePairingPriorityQueue:
			queue->push   = &PairingRPQ_push;
			queue->peek   = &PairingRPQ_peek;
			queue->pop    = &PairingRPQ_pop;
			queue->isEmpty= &__RPQ_Priv0Null;
			queue->clear  = &PairingRPQ_clear;
			queue->release= &PairingRPQ_release;
			break;
		default:
			return NULL;
	}
	return queue;
}
