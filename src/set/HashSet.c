#include <stdlib.h> //malloc, free

#include "Set.h"
#include "../common/Functions.h"

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
	struct HashSetNode* before;
	struct HashSetNode* after;
	struct HashSetNode* parent;
	struct HashSetNode* left;
	struct HashSetNode* right;
	struct HashSetNode* prev;
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
	struct TreeNodeData tree[];
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
	size_t resizeThreshold;
	/**
	 * The number of times that this set has been modified.
	 * Used for quick failure of iterators
	 */
	unsigned short modCount;
};

//Declared in Set.h
//Set* HashSet_create(Set* input, IntFunction* hashFn, Comparator* comparator);

//Internal methods
static struct HashSetNode* treeNodeFind(struct HashSetNode* root, Comparator* comparator, unsigned long hash, void* value) __attribute__ ((warn_unused_result, nonnull (1, 2)));
static struct HashSetNode* getTreeRoot(struct HashSetNode* base) __attribute__ ((warn_unused_result, nonnull (1)));
static struct HashSetNode* getNode(struct HashSetData* data, unsigned long hash, void* value) __attribute__ ((warn_unused_result, nonnnull (1)));
static bool HashSet_maybeResize(struct HashSetData* data) __attribute__ ((nonnnull (1)));

//HashSet methods
static bool      HashSet_add        (Set* self, void* value)      __attribute__ ((nonnull (1)));
static bool      HashSet_contains   (Set* self, void* value)      __attribute__ ((nonnull (1)));
static void*     HashSet_remove     (Set* self, void* value)      __attribute__ ((nonnull (1)));
static size_t    HashSet_getSize    (Set* self)                   __attribute__ ((nonnull (1)));
static Iterator* HashSet_iterator   (Set* self)                   __attribute__ ((nonnull (1)));
static void      HashSet_clear      (Set* self, Consumer* cleaner)__attribute__ ((nonnull (1)));
static void      HashSet_release    (Set* self)                   __attribute__ ((nonnull (1)));
static void      HashSet_releaseSelf(Set* self)                   __attribute__ ((nonnull (1)));


Set* HashSet_create(Set* input, IntFunction* hashFn, Comparator* comparator) {
	Set* set = input;
	if (set == NULL && (set = malloc(sizeof(Set))) == NULL) {
		return NULL;
	}
	//Try to allocate internal data
	if ((set->priv = malloc(sizeof(struct HashSetData))) == NULL) {
		//Alloc failed; free Set datastructure if we allocated it
		if (input == NULL)
			free(set);
		return NULL;
	}
	
	//Set up (punny :)) public methods
	set->add = &HashSet_add;
	set->contains = &HashSet_contains;
	set->remove = &HashSet_remove;
	set->getSize = &HashSet_getSize;
	set->iterator = &HashSet_iterator;
	set->clear = &HashSet_clear;
	set->release = input == NULL ? &HashSet_releaseSelf : &HashSet_release;
	
	//Initialize internal data
	struct HashSetData* data = (struct HashSetData*) set->priv;
	data->table = NULL;
	data->length = 0;
	data->capacity = 0;
	data->resizeThreshold = 0;
	
	return set;
}

static struct HashSetNode* treeNodeFind(struct HashSetNode* root, Comparator* comparator, unsigned long hash, void* value) {
	struct HashSetNode* current = root;
	do {
		unsigned long currentHash = current->hash;
		struct HashSetNode* left = current->tree[0].left;
		struct HashSetNode* right = current->tree[0].right;
		int comparison;
		if (currentHash > hash)
			current = left;
		else if (currentHash < hash)
			current = right;
		else if (current->value == value || (comparison = comparator->apply(comparator->priv, current->value, value)) == 0)
			return current;
		else if (left == NULL)
			current = right;
		else if (right == NULL || comparison < 0)
			current = left;
		else //if (comparison > 0)
			current = right;
	} while (current != NULL);
	return NULL;
}

static struct HashSetNode* getTreeRoot(struct HashSetNode* base) {
	struct HashSetNode* current = base;
	struct HashSetNode* parent;
	while ((parent = current->parent) != NULL)
		current = parent;
	return current;
}

static struct HashSetNode* getNode(struct HashSetData* data, unsigned long hash, void* value) {
	const struct HashSetNode* table = data->table;
	if (table == NULL)
		return NULL;
	const size_t capacity = data->capacity;
	if (capacity == 0)
		return NULL;
	//The first node should be the one we want, pretty often
	struct HashSetNode* first = table[(capacity - 1) & hash];
	if (first == NULL)
		return NULL;
	if (first->hash == hash && (first->value == value || data->comparator.apply(data->comparator.priv, first->value, value) == 0))
		return first;
	struct HashSetNode* current = first->next;
	if (current == NULL)
		return NULL;
	if (first->isTreeNode)
		return treeNodeFind(getTreeRoot(first), &(data->comparator), hash, value);
	do {
		if (current->hash == hash && (current->value == value || data->comparator.apply(data->comparator.priv, current->value, value) == 0))
			return current;
	} while ((current = current->next) != NULL);
	return NULL;
}

static bool HashSet_maybeResize(struct HashSetData* data) {
	struct HashSetNode** oldTable = data->table;
	const size_t oldCapacity = (oldTable == NULL) ? 0 : data->capacity;
	const size_t oldThreshold = data->resizeThreshold;
	const size_t length = data->length;
	if (length < oldThreshold)
		return true;
	size_t newCapacity
	size_t newThreshold = 0;
	if (oldCapacity > 0) {
		if (oldCapacity > HASHSET_MAXIMUM_CAPACITY) {
			data->resizeThreshold = SIZE_MAX;
			return true;
		}
		if ((newCapacity = oldCapacity << 1) < HASHSET_MAXIMUM_CAPACITY && oldCapacity > HASHSET_INITIAL_CAPACITY)
			newThreshold = oldThreshold << 1;
	} else if (oldThreshold > 0) {
		newCapacity = oldThreshold;
	} else {
		newCapacity = HASHSET_INITIAL_CAPACITY;
		newThreshold = HASHSET_INITIAL_CAPACITY * 3/4;
	}
	if (newThreshold == 0)
		newThreshold = newCapacity < HASHSET_MAXIMUM_CAPACITY ? (newCapacity * 3/4) : SIZE_MAX;
	struct HashSetNode** newTable = calloc(newCapacity, sizeof(struct HashSetNode*));
	if (newTable == NULL)
		//Error: out of memory
		return false;
	data->resizeThreshold = newThreshold;
	data->table = newTable;
	if (oldTable != NULL) {
		for (size_t i = 0; i < oldCapacity; i++) {
			struct HashSetNode* current = oldTable[i];
			if (current == NULL)
				continue;
			if (current->next == NULL) {
				newTable[(newCapacity - 1) & current->hash] = current;
			} else if (current->isTreeNode) {
				//Relink tree nodes into low and high lists
				//This operation preserves the order of the nodes
				struct HashSetNode* next;
				struct HashSetNode* loHead = NULL;
				struct HashSetNode* loTail = NULL;
				struct HashSetNode* hiHead = NULL;
				struct HashSetNode* hiTail = NULL;
				//Store the size of both of the lists, so we can know whether to un-treeify them
				int loCount = 0, hiCount = 0;
				for (; current != NULL; current = next) {
					next = current->tree[0].next;
					current->tree[0].next = NULL;
					if ((current->hash & oldCapacity) == 0) {
						if ((current->tree[0].prev = loTail) == NULL)
							loHead = current;
						else
							loTail->next = current;
						loTail = current;
						loCount++;
					} else {
						if ((current->tree[0].prev = hiTail) == NULL)
							hiHead = current;
						else
							hiTail->next = current;
						hiTail = current;
						hiCount++;
					}
				}
				
				if (loHead != NULL) {
					if (loCount <= HASHSET_UNTREE_THRESHOLD)
						newTable[i] = //untreeify loHead
					else {
						newTable[i] = loHead;
						if (hiHead != NULL)
							//treeify loHead
					}
				}
				if (hiHead != NULL) {
					if (hiCount <= HASHSET_UNTREE_THRESHOLD)
						newTable[i + oldCapacity] = //untreeify hiHead
					else {
						newTable[i + oldCapacity] = hiHead;
						if (loHead != NULL)
							//treeify loHead
					}
				}
			} else {
				//Linked list of nodes that had different hashes that were previously masked
				struct HashSetNode* next;
				struct HashSetNode* loHead = NULL;
				struct HashSetNode* loTail = NULL;
				struct HashSetNode* hiHead = NULL;
				struct HashSetNode* hiTail = NULL;
				do {
					next = current->next;
					if ((current->hash & oldCapacity) == 0) {
						if (loTail == NULL)
							loHead = current;
						else
							loTail->next = current;
						loTail = current;
					} else {
						if (hiTail == NULL)
							hiHead = current;
						else
							hiTail.next = current;
						hiTail = current;
					}
				} while ((current = next) != NULL);
				
				if (loTail != NULL) {
					loTail->next = NULL;
					newTable[i] = loHead;
				}
				if (hiTail != NULL) {
					hiTail->next = NULL;
					newTable[i + oldCapacity] = hiHead;
				}
			}
		}
		free(oldTable);
	}
}

bool HashSet_add(Set* self, void* value) {
	struct HashSetData* data = (struct HashSetData*) self->priv;
	struct HashSetNode** table = data->table;
	size_t capacity = data->capacity;
	if (table == NULL || capacity == 0)
		HashSet_maybeResize(data);
	
	const unsigned long hash = data->hashFn->apply(data->hashFn->priv, value);
	
	struct HashSetNode* first = table[(capacity - 1) & hash];
	if (first == NULL) {
		first = table[(capacity - 1) & hash] = malloc(sizeof(struct HashSetNode));
		first->hash = hash;
		first->value = value;
		first->next = NULL;
		first->isTreeNode = false;
		data->modCount++;
		data->length++;
		HashSet_maybeResize(data);
		return true;
	}
	
	if (first->hash == hash && (first->value == value || data->comparator.apply(data->comparator.priv, first->value, value) == 0))
		return false; //Entry already exists in set
	
	
	return false;
}

bool HashSet_contains(Set* self, void* value) {
	struct HashSetData* data = (struct HashSetData*) self->priv;
	unsigned long hash = data->hashFn->apply(data->hashFn->priv, value);
	return getNode(data, hash, value) != NULL;
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
	struct HashSetData* data = (struct HashSetData*) self->priv;
	struct HashSetNode** table = data->table;
	const size_t length = data->length;
	if (table != NULL && length > 0) {
		data->length = 0;
		
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
