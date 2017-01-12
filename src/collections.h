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

/**
 * A comparison function that is able to produce a total ordering of a set of elements.
 * Care should be given such that the ordering of all elements is consitent (for example,
 * if cmp(a,b) > 0, cmp(b,c) > 0, then cmp(a,c) must > 0).
 * Returns a positive number if a > b
 * Returns 0 if a is equivalent to b
 * Returns a negative number if a < b
 */
typedef int (Comparator)(void* a, void* b);
/**
 * A method that safely releases the passed element.
 * Intentionally the same signature as free().
 */
typedef void (Cleaner) (void* value);

#endif