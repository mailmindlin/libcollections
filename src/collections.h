#ifndef __LIBCOLLECTIONS_COLLECTIONS_H
#define __LIBCOLLECTIONS_COLLECTIONS_H

typedef enum {
	UnknownCollectionType,
	LinkedFIFO,
	LinkedLIFO,
	FibonacciRPQ,
	PairingRPQ,
	FibonacciKPQ,
	PairingKPQ,
	LinkedList,
	ArrayList
} CollectionType;

#endif