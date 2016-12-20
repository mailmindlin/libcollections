#ifndef __LIBCOLLECTIONS_COLLECTIONS_H
#define __LIBCOLLECTIONS_COLLECTIONS_H

typedef enum {
	UnknownCollectionType,
	FibonacciQueue,
	PairingQueue,
	LinkedQueue,
	RelativeFibonacciPriorityQueue,
	RelativePairingPriorityQueue,
	RelativeLinkedPriorityQueue,
	KeyedFibonacciPriorityQueue,
	KeyedPairingPriorityQueue
} CollectionType;

#endif