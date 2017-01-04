#include <stddef.h>	//For NULL
#include <stdlib.h>	//For malloc, free
#include "queue.h"
#include "queue-priv.h"

//Pairing heap implementation
//see https://users.cs.fiu.edu/~weiss/dsaa_c++/code/PairingHeap.cpp, https://www.cs.cmu.edu/~sleator/papers/pairing-heaps.pdf

static PairingRPQNode* doMerge(PairingRPQNode* treeA, PairingRPQNode* treeB, Comparator* comparator) {
	if (treeA == NULL)
		return treeB;
	if (treeB == NULL)
		return treeA;
	
	if (comparator(treeA->value, treeB->value) > 0) {
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

static PairingRPQNode* combineSibilings(PairingRPQNode* firstSibiling, Comparator* comparator) {
	if (firstSibiling == NULL || firstSibiling->sibiling == NULL)
		return firstSibiling;
	size_t treeArrayCapacity = 8;
	PairingRPQNode** treeArray = malloc(8 * sizeof(PairingRPQNode*));
	if (!treeArray) {
		//OOM error
		return NULL;
	}
	size_t numPairs = 0;
	PairingRPQNode* currentNode = firstSibiling;
	//Fill treeArray with pairs of nodes, going left-to-right.
	do {
		if (numPairs >= treeArrayCapacity) {
			size_t newTAC = treeArrayCapacity * 2 / 3 + 1;
			PairingRPQNode** newTreeArray = realloc(treeArray, newTAC * sizeof(PairingRPQNode*));
			if (newTreeArray == NULL) {
				//OOM error
				free(treeArray);
				return NULL;
			}
			treeArrayCapacity = newTAC;
			treeArray = newTreeArray;
		}
		PairingRPQNode* sibilingA = currentNode;
		PairingRPQNode* sibilingB = sibilingA->sibiling;
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
	PairingRPQNode* lastPair = treeArray[numPairs - 1];
	numPairs--;
	for(;numPairs > 0; numPairs--)
		lastPair = doMerge(treeArray[numPairs - 1], lastPair, comparator);
	free(treeArray);
	return lastPair;
}

static void doReleaseNode(PairingRPQNode* node, Cleaner* cleaner) {
	PairingRPQNode* current = node;
	while (current != NULL) {
		PairingRPQNode* sibiling = current->sibiling;
		PairingRPQNode* child = current->child;
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

bool PairingRPQ_empty(Queue* queue) {
	return queue->rpq.pairingRPQData.root == NULL;
}

void* PairingRPQ_peek(Queue* queue) {
	if (queue->rpq.pairingRPQData.root == NULL)
		//Underflow
		return NULL;
	return queue->rpq.pairingRPQData.root->value;
}

void* PairingRPQ_pop(Queue* queue) {
	PairingRPQNode* oldRoot = queue->rpq.pairingRPQData.root;
	if (oldRoot == NULL)
		//Underflow
		return NULL;
	void* result = oldRoot->value;
	PairingRPQNode* firstChild = oldRoot->child;
	free(oldRoot);
	queue->rpq.pairingRPQData.root = combineSibilings(firstChild, queue->rpq.comparator);
	return result;
}

bool PairingRPQ_push(Queue* queue, void* value) {
	PairingRPQNode* node = malloc(sizeof(PairingRPQNode));
	if (node == NULL)
		return false;
	node->child = NULL;
	node->sibiling = NULL;
	node->value = value;
	queue->rpq.pairingRPQData.root = doMerge(queue->rpq.pairingRPQData.root, node, queue->rpq.comparator);
	return true;
}

void PairingRPQ_clear(Queue* queue, Cleaner* cleaner) {
	PairingRPQNode* root = queue->rpq.pairingRPQData.root;
	doReleaseNode(root, cleaner);
}

void PairingRPQ_release(Queue* queue, Cleaner* cleaner) WEAKREF(PairingRPQ_clear);