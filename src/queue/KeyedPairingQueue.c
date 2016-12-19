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
	unsigned int key;
	void *value;
};

static PriorityQueueNode* doMerge(PriorityQueueNode* treeA, PriorityQueueNode* treeB) {
	if (treeA == NULL)
		return treeB;
	if (treeB == NULL)
		return treeA;
	
	if (treeA->key > treeB->key) {
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

static PriorityQueueNode* combineSibilings(PriorityQueueNode* firstSibiling) {
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
		treeArray[numPairs++] = doMerge(sibilingA, sibilingB);
	} while (currentNode != NULL);
	
	//Merge/accumulate remaining pairs right-to-left
	PriorityQueueNode* lastPair = treeArray[numPairs - 1];
	numPairs--;
	for(;numPairs > 0; numPairs--)
		lastPair = doMerge(treeArray[numPairs - 1], lastPair);
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

unsigned int PairingKPQ_peekKey(KeyedPriorityQueue* queue) {
	if (queue->priv[0] == NULL)
		//Underflow
		return 0;
	return ((PriorityQueueNode*)queue->priv[0])->key;
}

void* PairingKPQ_peek(KeyedPriorityQueue* queue) {
	if (queue->priv[0] == NULL)
		//Underflow
		return NULL;
	return ((PriorityQueueNode*)queue->priv[0])->value;
}

void* PairingKPQ_pop(KeyedPriorityQueue* queue) {
	PriorityQueueNode* oldRoot = ((PriorityQueueNode*)queue->priv[0]);
	if (oldRoot == NULL)
		//Underflow
		return NULL;
	void* result = oldRoot->value;
	PriorityQueueNode* firstChild = oldRoot->child;
	free(oldRoot);
	queue->priv[0] = (void*) combineSibilings(firstChild);
	return result;
}

bool PairingKPQ_push(KeyedPriorityQueue* queue, unsigned int key, void* value) {
	PriorityQueueNode* node = malloc(sizeof(PriorityQueueNode));
	if (node == NULL)
		return false;
	node->parent = NULL;
	node->child = NULL;
	node->sibiling = NULL;
	node->key = key;
	node->value = value;
	PriorityQueueNode* root = ((PriorityQueueNode*)queue->priv[0]);
	root = doMerge(root, node);
	queue->priv[0] = (void*) root;
	return true;
}

void PairingKPQ_clear(KeyedPriorityQueue* queue, Cleaner* cleaner) {
	PriorityQueueNode* root = ((PriorityQueueNode*)queue->priv[0]);
	doReleaseNode(root, cleaner);
}

void PairingKPQ_release(KeyedPriorityQueue* queue, Cleaner* cleaner) {
	PairingKPQ_clear(queue, cleaner);
}
