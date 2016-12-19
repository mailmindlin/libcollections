#ifndef __LIBCOLLECTIONS_COLLECTIONS_H
#define __LIBCOLLECTIONS_COLLECTIONS_H

typedef enum {
	UnknownCollectionType,
	FibonacciQueue,
	PairingQueue,
	RelativeFibonacciPriorityQueue,
	RelativePairingPriorityQueue,
	KeyedFibonacciPriorityQueue,
	KeyedPairingPriorityQueue
} CollectionType;

#endif