#include <stdbool.h>
#include "../collections.h"
#include "../locality.h"
#ifndef __LIBCOLLECTIONS_QUEUE_H
#define __LIBCOLLECTIONS_QUEUE_H

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

struct PairingRPQNode;
struct PairingKPQNode;
struct FibonacciRPQNode;
struct FibonacciKPQNode;
struct SinglyLinkedNode;

/**
 * A collection for which its elements are held in a specific order.
 */
typedef struct Queue {
	/**
	 * Inserts the specified element into this queue if immediately possible (without
	 * violating capacity restrictions, etc.) Returns true on success, false on error.
	 */
	bool (*push) (struct Queue* queue, void* value);
	/**
	 * Retrieves, without removing, the head of the queue, or NULL if this queue is
	 * empty.
	 */
	void* (*peek) (struct Queue* queue);
	/**
	 * Retrieves and removes the head of the queue, or returns NULL if this queue is
	 * empty.
	 */
	void* (*pop) (struct Queue* queue);
	/**
	 * Returns true if this queue has no elements
	 */
	bool (*isEmpty) (struct Queue* queue);
	/**
	 * Removes all elements from this queue, optionally applying the given cleaner to
	 * them.
	 * 
	 * Note that after this method is called, isEmpty MUST return true, but this
	 * queue's data structure MAY NOT be safely released.
	 */
	void (*clear) (struct Queue* queue, Cleaner* cleaner);
	/**
	 * Removes all elements from this queue, optionally applying the given cleaner to
	 * them.
	 * All internal memory MUST be released, and after this method is called, this
	 * queue's structure should be safe to free.
	 */
	void (*release) (struct Queue* queue, Cleaner* cleaner);
	/**
	 * The implementation type of this queue. Please don't set.
	 */
	CollectionType type;
	//Implementation-specific data. Please don't touch.
	union {
		union {
			struct {
				struct SinglyLinkedNode* head;
			} linkedFIFOData;
		} fifo;
		union {
			struct {
				struct SinglyLinkedNode* head;
				struct SinglyLinkedNode* tail;
			} linkedLIFOData;
		} lifo;
		struct {
			Comparator* comparator;
			union {
				struct {
					struct PairingRPQNode* root;
				} pairingRPQData;
				struct {
					struct FibonacciRPQNode* root;
				} fibonacciRPQData;
			};
		} rpq;
	};
} Queue;

typedef struct KeyedPriorityQueue {
	bool (*push) (struct KeyedPriorityQueue* queue, unsigned int key, void* value);
	void* (*peek) (struct KeyedPriorityQueue* queue);
	unsigned int (*peekKey) (struct KeyedPriorityQueue* queue);
	void* (*pop) (struct KeyedPriorityQueue* queue);
	bool (*isEmpty) (struct KeyedPriorityQueue* queue);
	void (*clear) (struct KeyedPriorityQueue* queue, Cleaner* cleaner);
	void (*release) (struct KeyedPriorityQueue* queue, Cleaner* cleaner);
	CollectionType type;
	//Implementation-specific data. Please don't touch.
	union {
		struct {
			struct PairingKPQNode* root;
		} pairingKPQData;
		struct {
			struct FibonacciKPQNode* root;
		} fibonacciKPQData;
	};
} KeyedPriorityQueue;

/**
 * Initialize a 'boring' queue. Supported CollectionType's are:
 *  - LinkedFIFO (default)
 *      A FIFO queue based on a singly-linked-list implementation.
 *  - LinkedLIFO
 *      A LIFO queue based on a singly-linked-list implementation.
 * If UnknownCollectionType is given, it will be treated as a LinkedFIFO.
 * Returns the initialized queue on success, else NULL.
 */
Queue* LIBCOLLECTIONS_PUBLIC InitQueue(Queue* queue, CollectionType type);
/**
 * Initialize a relative priority queue. Instead of a 'normal' FIFO/LIFO queue that
 * orders elements by their insertion order, relative priority queues order them using
 * the given comparator.
 * 
 * Supported CollectionType's are:
 *  - PriorityRPQ (default)
 *      A relative priority queue based on a priority heap
 *  - FibonacciRPQ
 *      A relative priority queue based on a fibonacci heap.
 * Returns the initialized queue on success, else NULL.
 */
Queue* LIBCOLLECTIONS_PUBLIC InitRelativePriorityQueue(Queue* queue, CollectionType type, Comparator* comparator);
/**
 * Initialize a keyed priority queue. Similar to a relative priority queue,
 * it differs in that instead of a comparator, an absolute priority is given on every
 * element's insertion.
 * Supported CollectionType's are:
 *  - PriorityKPQ (default)
 *      A relative priority queue based on a priority heap
 *  - FibonacciKPQ
 *      A relative priority queue based on a fibonacci heap.
 * Returns the initialized queue on success, else NULL.
 */
KeyedPriorityQueue* LIBCOLLECTIONS_PUBLIC InitKeyedPriorityQueue(KeyedPriorityQueue* queue, CollectionType type);

#endif
