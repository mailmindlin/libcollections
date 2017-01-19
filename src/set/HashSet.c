#include "Set.h"
#include "../common/Functions.h"
#include <stdlib.h> //malloc, free

#define HASHSET_INITIAL_CAPACITY  (1 << 4)
#define HASHSET_MAXIMUM_CAPACITY  (1 << 30)
#define HASHSET_TREE_THRESHOLD    8
#define HASHSET_UNTREE_THRESHOLD  6
#define HASHSET_MIN_TREE_CAPACITY 64

struct HashSetNode;
struct TreeNodeData {
	/**
	 * All guaranteed to be a tree node (if not null)
	 */
	HashSetNode* before;
	HashSetNode* after;
	HashSetNode* parent;
	HashSetNode* left;
	HashSetNode* right;
	HashSetNode* prev;
	bool red;
};

struct HashSetNode {
	unsigned long hash;
	void* value;
	struct HashSetNode* next;
	bool isTreeNode;
	/**
	 * We can leverage C99/C11's flexible array members to tack on more data
	 * if this is a tree node.
	 * This array has either 0 or 1 elements (1 iff isTreeNode == true).
	 * See stackoverflow.com/a/32311914/2759984
	 */
	struct TreeNodeData[];
};

struct HashSetData {
	IntFunction hashFn;
	Comparator comparator;
	struct HashSetNode** table;
	/**
	 * Capacity of table
	 */
	size_t capacity;
	/**
	 * The number of entries in this set
	 */
	size_t length;
	/**
	 * The number of times that this set has been modified.
	 * Used for quick failure of iterators
	 */
	unsigned short modCount;
};

Set* HashSet_create(Set* input, IntFunction* hashFn, Comparator* comparator) {
	Set* result = input;
	if (result == NULL && (result = malloc(sizeof(Set))) == NULL) {
		return NULL;
	}
	if ((result->priv = malloc(sizeof(struct HashSetData))) == NULL) {
		if (input == NULL)
			free(result);
		return NULL;
	}
	
	set->add = &HashSet_add;
	set->contains = &HashSet_contains;
	set->remove = &HashSet_remove;
	set->getSize = &HashSet_getSize;
	set->iterator = &HashSet_iterator;
	set->clear = &HashSet_clear;
	set->release = input == NULL ? &HashSet_releaseSelf : &HashSet_release;
	
	struct HashSetData* data = (struct HashSetData*) result->priv;
	return result;
}

static struct HashSetNode* getTreeRoot(struct HashSetNode* base) {
	struct HashSetNode* current = base;
	struct HashSetNode* parent;
	while ((parent = current->parent) != NULL)
		current = parent;
	return current;
}
/**
 * Searches a tree.
 * Prerequisites:
 *       base is a tree node
 */
static struct HashSetNode* getTreeNode(struct HashSetNode* base, unsigned long hash, void* value) {
	HashSetNode* root = getTreeRoot(base);
	//TODO finish
}

static struct HashSetNode* getNode(struct HashSetData* data, unsigned long hash, void* value) {
	const struct HashSetNode* table = data->table;
	if (table == NULL)
		return NULL;
	const size_t capacity = data->capacity;
	if (capacity == 0)
		return NULL;
	struct HashSetNode* first = table[(capacity - 1) & hash];
	if (first == NULL)
		return NULL;
	if (first->hash == hash && (first->value == value || data->comparator.apply(data->comparator.priv, first->value, value) == 0))
		return first;
	struct HashSetNode* current = first->next;
	if (current == NULL)
		return NULL;
	if (first->isTreeNode)
		return getTreeNode(data, first, hash, value);
	do {
		if (current->hash == hash && (current->value == value || data->comparator.apply(data->comparator.priv, current->value, value) == 0))
			return current;
	} while ((current = current->next) != NULL);
	return NULL;
}

bool HashSet_add(Set* self, void* value) {
	struct HashSetData* data = (struct HashSetData*) self->priv;
	unsigned long hash = data->hashFn->apply(data->hashFn->priv, value);
	
	return false;
}

bool HashSet_contains(Set* self, void* value) {
	struct HashSetData* data = (struct HashSetData*) self->priv;
	unsigned long hash = data->hashFn->apply(data->hashFn->priv, value);
	return getTreeNode(data, hash, value) == NULL;
}

void* HashSet_remove(Set* self, void* value) {
	return NULL;
}

size_t HashSet_getSize(Set* self) {
	struct HashSetData* data = (struct HashSetData*) self->priv;
	return data->length;
}

Iterator* HashSet_iterator(Set* self) {
	return NULL;
}

void HashSet_clear(Set* self, Consumer* cleaner) {
	if (cleaner != NULL) {
		
	}
}

void HashSet_release(Set* self) {
	struct HashSetData* data = (struct HashSetData*) self->priv;
	data->hashFn.release(data->hashFn.priv);
	data->comparator.release(data->comparator.priv);
	//TODO free elements
	free(data);
}

void HashSet_releaseSelf(Set* self) {
	HashSet_release(self);
	free(self);
}
