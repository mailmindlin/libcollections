#include <stddef.h>	//For NULL
#include <stdlib.h>	//For malloc, free
#include "queue.h"
#include "queue-priv.h"

//Pairing heap implementation
//see https://users.cs.fiu.edu/~weiss/dsaa_c++/code/PairingHeap.cpp, https://www.cs.cmu.edu/~sleator/papers/pairing-heaps.pdf
struct PriorityQueueNode_s;
typedef struct PriorityQueueNode_s PriorityQueueNode;
struct PriorityQueueNode_s {
	PriorityQueueNode* parent;
	PriorityQueueNode* child;
	PriorityQueueNode* sibiling;
	void *value;
};

static PriorityQueueNode* doMerge(PriorityQueueNode* treeA, PriorityQueueNode* treeB, Comparator* comparator) {
	if (treeA == NULL)
		return treeB;
	if (treeB == NULL)
		return treeA;
	
	if (comparator(treeA->value, treeB->value) > 0) {
		//Attach treeA as child of treeB
		treeB->parent = treeA->parent;
		treeA->parent = treeB;
		treeA->sibiling = treeB->child;
		//?
		if (treeA->sibiling != NULL)
			treeA->sibiling->parent = treeA;
		treeB->child = treeA;
		return treeB;
	} else {
		//Attach treeB as child of treeA
		treeB->parent = treeA;
		treeA->sibiling = treeB->sibiling;
		if (treeA->sibiling != NULL)
			treeA->sibiling->parent = treeA;
		treeB->sibiling = treeA->child;
		treeA->child = treeB;
		return treeA;
	}
}

static PriorityQueueNode* combineSibilings(PriorityQueueNode* firstSibiling, Comparator* comparator) {
	if (firstSibiling == NULL || firstSibiling->sibiling == NULL)
		return firstSibiling;
	size_t treeArrayCapacity = 8;
	PriorityQueueNode** treeArray = malloc(8 * sizeof(PriorityQueueNode*));
	if (!treeArray) {
		//OOM error
		return NULL;
	}
	size_t numPairs = 0;
	PriorityQueueNode* currentNode = firstSibiling;
	//Fill treeArray with pairs of nodes, going left-to-right.
	do {
		if (numPairs >= treeArrayCapacity) {
			size_t newTAC = treeArrayCapacity * 2 / 3 + 1;
			PriorityQueueNode** newTreeArray = realloc(treeArray, newTAC * sizeof(PriorityQueueNode*));
			if (newTreeArray == NULL) {
				//OOM error
				free(treeArray);
				return NULL;
			}
			treeArrayCapacity = newTAC;
			treeArray = newTreeArray;
		}
		PriorityQueueNode* sibilingA = currentNode;
		PriorityQueueNode* sibilingB = sibilingA->sibiling;
		sibilingA->sibiling = NULL;
		if (sibilingB == NULL) {
			//Last sibiling (odd #, no other to pair it with)
			treeArray[numPairs++] = sibilingA;
			break;
		}
		currentNode = sibilingB->sibiling;
		sibilingB->sibiling = NULL;
		treeArray[numPairs++] = doMerge(sibilingA, sibilingB, comparator);
	} while (currentNode != NULL);
	
	//Merge/accumulate remaining pairs right-to-left
	PriorityQueueNode* lastPair = treeArray[numPairs - 1];
	numPairs--;
	for(;numPairs > 0; numPairs--)
		lastPair = doMerge(treeArray[numPairs - 1], lastPair, comparator);
	free(treeArray);
	return lastPair;
}

static void doReleaseNode(PriorityQueueNode* node, Cleaner* cleaner) {
	PriorityQueueNode* current = node;
	while (current != NULL) {
		PriorityQueueNode* sibiling = current->sibiling;
		PriorityQueueNode* child = current->child;
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

void* PairingRPQ_peek(RelativePriorityQueue* queue) {
	if (queue->priv[0] == NULL)
		//Underflow
		return NULL;
	return ((PriorityQueueNode*)queue->priv[0])->value;
}

void* PairingRPQ_pop(RelativePriorityQueue* queue) {
	PriorityQueueNode* oldRoot = ((PriorityQueueNode*)queue->priv[0]);
	if (oldRoot == NULL)
		//Underflow
		return NULL;
	void* result = oldRoot->value;
	PriorityQueueNode* firstChild = oldRoot->child;
	free(oldRoot);
	queue->priv[0] = (void*) combineSibilings(firstChild, queue->comparator);
	return result;
}

bool PairingRPQ_push(RelativePriorityQueue* queue, void* value) {
	PriorityQueueNode* node = malloc(sizeof(PriorityQueueNode));
	if (node == NULL)
		return false;
	node->parent = NULL;
	node->child = NULL;
	node->sibiling = NULL;
	node->value = value;
	PriorityQueueNode* root = ((PriorityQueueNode*)queue->priv[0]);
	root = doMerge(root, node, queue->comparator);
	queue->priv[0] = (void*) root;
	return true;
}

void PairingRPQ_clear(RelativePriorityQueue* queue, Cleaner* cleaner) {
	PriorityQueueNode* root = ((PriorityQueueNode*)queue->priv[0]);
	doReleaseNode(root, cleaner);
}

void PairingRPQ_release(RelativePriorityQueue* queue, Cleaner* cleaner) {
	PairingRPQ_clear(queue, cleaner);
}
