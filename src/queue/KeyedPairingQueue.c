#include <stddef.h>	//For NULL
#include <stdlib.h>	//For malloc, free
#include "queue.h"
#include "queue-priv.h"

/**
 * Pairing heap implementation.
 * 
 * Pairing heaps are weird because (at the time of writing) nobody is really
 * sure how efficient they really are, but in practice, they seem to perform
 * better than most other queues.
 *
 * See also:
 *  - en.wikipedia.org/wiki/Pairing_heap
 *  - users.cs.fiu.edu/~weiss/dsaa_c++/code/PairingHeap.cpp
 *  - www.cs.cmu.edu/~sleator/papers/pairing-heaps.pdf
 */

static PairingKPQNode* doMerge(PairingKPQNode* treeA, PairingKPQNode* treeB) {
	if (treeA == NULL)
		return treeB;
	if (treeB == NULL)
		return treeA;
	
	if (treeA->key > treeB->key) {
		//Attach treeA as child of treeB
		treeA->sibiling = treeB->child;
		treeB->child = treeA;
		return treeB;
	} else {
		//Attach treeB as child of treeA
		treeA->sibiling = treeB->sibiling;
		treeB->sibiling = treeA->child;
		treeA->child = treeB;
		return treeA;
	}
}

static PairingKPQNode* combineSibilings(PairingKPQNode* firstSibiling) {
	if (firstSibiling == NULL || firstSibiling->sibiling == NULL)
		return firstSibiling;
	size_t treeArrayCapacity = 8;
	PairingKPQNode** treeArray = malloc(8 * sizeof(PairingKPQNode*));
	if (!treeArray) {
		//OOM error
		return NULL;
	}
	size_t numPairs = 0;
	PairingKPQNode* currentNode = firstSibiling;
	//Fill treeArray with pairs of nodes, going left-to-right.
	do {
		if (numPairs >= treeArrayCapacity) {
			size_t newTAC = treeArrayCapacity * 2 / 3 + 1;
			PairingKPQNode** newTreeArray = realloc(treeArray, newTAC * sizeof(PairingKPQNode*));
			if (newTreeArray == NULL) {
				//OOM error
				free(treeArray);
				return NULL;
			}
			treeArrayCapacity = newTAC;
			treeArray = newTreeArray;
		}
		PairingKPQNode* sibilingA = currentNode;
		PairingKPQNode* sibilingB = sibilingA->sibiling;
		sibilingA->sibiling = NULL;
		if (sibilingB == NULL) {
			//Last sibiling (odd #, no other to pair it with)
			treeArray[numPairs++] = sibilingA;
			break;
		}
		currentNode = sibilingB->sibiling;
		sibilingB->sibiling = NULL;
		treeArray[numPairs++] = doMerge(sibilingA, sibilingB);
	} while (currentNode != NULL);
	
	//Merge/accumulate remaining pairs right-to-left
	PairingKPQNode* lastPair = treeArray[numPairs - 1];
	numPairs--;
	for(;numPairs > 0; numPairs--)
		lastPair = doMerge(treeArray[numPairs - 1], lastPair);
	free(treeArray);
	return lastPair;
}

static void doReleaseNode(PairingKPQNode* node, Cleaner* cleaner) {
	/*
	 * TODO look at/profile whether it is faster in practice to
	 * traverse sibilings or children (i.e., whether any given node 
	 * is more likely to have a child or a sibiling); should improve
	 * performance at least a bit, and help reduce stack overflows
	 */
	PairingKPQNode* current = node;
	while (current != NULL) {
		PairingKPQNode* sibiling = current->sibiling;
		PairingKPQNode* child = current->child;
		cleaner(current->value);
		free(current);
		current = sibiling;
		if (child != NULL) {
			if (current != NULL)
				doReleaseNode(child, cleaner);
			else
				current = child;
		}
	}
}

bool PairingKPQ_empty(KeyedPriorityQueue* queue) {
	return queue->pairingKPQData.root == NULL;
}

unsigned int PairingKPQ_peekKey(KeyedPriorityQueue* queue) {
	if (queue->pairingKPQData.root == NULL)
		//Underflow
		return 0;
	return queue->pairingKPQData.root->key;
}

void* PairingKPQ_peek(KeyedPriorityQueue* queue) {
	if (queue->pairingKPQData.root == NULL)
		//Underflow
		return NULL;
	return queue->pairingKPQData.root->value;
}

void* PairingKPQ_pop(KeyedPriorityQueue* queue) {
	PairingKPQNode* oldRoot = queue->pairingKPQData.root;
	if (oldRoot == NULL)
		//Underflow
		return NULL;
	void* result = oldRoot->value;
	PairingKPQNode* firstChild = oldRoot->child;
	free(oldRoot);
	queue->pairingKPQData.root = combineSibilings(firstChild);
	return result;
}

bool PairingKPQ_push(KeyedPriorityQueue* queue, unsigned int key, void* value) {
	PairingKPQNode* node = malloc(sizeof(PairingKPQNode));
	if (node == NULL)
		return false;
	node->child = NULL;
	node->sibiling = NULL;
	node->key = key;
	node->value = value;
	queue->pairingKPQData.root = doMerge(queue->pairingKPQData.root, node);
	return true;
}

void PairingKPQ_clear(KeyedPriorityQueue* queue, Cleaner* cleaner) {
	PairingKPQNode* root = queue->pairingKPQData.root;
	doReleaseNode(root, cleaner);
}

void PairingKPQ_release(KeyedPriorityQueue* queue, Cleaner* cleaner) WEAKREF(PairingKPQ_clear);