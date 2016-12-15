#include "queue.h"
//Pairing heap implementation
//see https://users.cs.fiu.edu/~weiss/dsaa_c++/code/PairingHeap.cpp, https://www.cs.cmu.edu/~sleator/papers/pairing-heaps.pdf
struct PriorityQueueNode_s;
typedef struct PriorityQueueNode_s PriorityQueueNode;
struct {
	PriorityQueueNode* parent;
	PriorityQueueNode* child;
	PriorityQueueNode* sibiling;
	unsigned int key;
	void *value;
} PriorityQueueNode_s;

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

bool PriorityQueue_init(RelativePriorityQueue* queue) __attribute((nonnull(1)) {
	queue->priv[0] = NULL;
	return true;
}

PriorityQueue* PriorityQueue_alloc() __attribute__((malloc)) {
	PriorityQueue* result = malloc(sizeof(PriorityQueue));
	if (result == NULL)
		//Malloc error
		return NULL;
	if (!PriorityQueue_init(result)) {
		free(result);
		return NULL;
	}
	return result;
}
unsigned int PriorityQueue_findMinimumKey(PriorityQueue* queue) __attribute__((const, nonnull(1))) {
	if (queue->root == NULL)
		//Underflow
		return 0;
	return queue->root->key;
}
void* PriorityQueue_findMinimumValue(PriorityQueue* queue) __attribute__((const, nonnull(1))) {
	if (queue->root == NULL)
		//Underflow
		return NULL;
	return queue->root->value;
}
void* PriorityQueue_deleteMinimum(PriorityQueue* queue) __attribute__((const, nonnull(1))) {
	if (queue->root == NULL)
		//Underflow
		return NULL;
	PriorityQueueNode* oldRoot = queue->root;
	void* result = queue->root->value;
	PriorityQueueNode* firstChild = queue->root->child;
	free(queue->root);
	queue->root = combineSibilings(firstChild);
	return result;
}
bool PriorityQueue_insert(PriorityQueue* queue, unsigned int key, void* value) __attribute__((const, nonnull(1))) {
	PriorityQueueNode* node = malloc(sizeof(PriorityQueueNode));
	if (node == NULL)
		return false;
	node->parent = NULL;
	node->child = NULL;
	node->sibiling = NULL;
	node->key = key;
	node->value = value;
	queue->root = doMerge(queue->root, node);
	return true;
}
bool PriorityQueue_isEmpty(PriorityQueue* queue) __attribute__((const, nonnull(1))) {
	return queue->root == NULL;
}
void PriorityQueue_deleteAll(PriorityQueue* queue) __attribute__((const, nonnull(1))) {
	
}
void PriorityQueue_release(PriorityQueue* queue) __attribute__((const, nonnull(1)) {
	PriorityQueue_deleteAll(queue);
	free(queue);
}