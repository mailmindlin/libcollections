#include <errno.h> //For errno
#include <stdlib.h>//For realloc, free
#include <stddef.h>//For size_t

#include "list.h"

//ArrayList internal methods
static bool ArrayList_grow(List* list, const size_t minCap) __attribute__ ((nonnull (1)));
static void ArrayList_shrink(List* list) __attribute__ ((nonnull (1)));

//Arraylist iterator methods
static bool ArrayList_iterator_hasNext(Iterator* self) __attribute__ ((nonnull (1)));
static void* ArrayList_iterator_next(Iterator* self) __attribute__ ((nonnull (1)));
static void* ArrayList_iterator_remove(Iterator* self) __attribute__ ((nonnull (1)));

//ArrayList public methods
static size_t ArrayList_add     (List* list, void* value) __attribute__ ((nonnull (1)));
static void*  ArrayList_set     (List* list, size_t index, void* value) __attribute__ ((nonnull (1)));
static void*  ArrayList_get     (List* list, size_t index) __attribute__ ((nonnull (1)));
static void*  ArrayList_remove  (List* list, size_t index) __attribute__ ((nonnull (1)));
static Iterator* ArrayList_iterator(List* list) __attribute__ ((nonnull (1)));
static size_t ArrayList_size    (List* list) __attribute__ ((nonnull (1)));
static void   ArrayList_clear   (List* list, Cleaner* cleaner) __attribute__ ((nonnull (1)));
static void   ArrayList_release (List* list, Cleaner* cleaner) __attribute__ ((nonnull (1)));
static void   ArrayList_releaseSelf(List* list, Cleaner* cleaner) __attribute__ ((nonnull (1)));


/**
 * Initializes ArrayList
 */
List* ArrayList_create(List* list) {
	List* result = list;
	if (result == NULL) {
		if ((result = malloc(sizeof(List))) == NULL) {
			errno = ENOMEM;
			return NULL;
		}
	}
	
	list->add =      &ArrayList_add;
	list->set =      &ArrayList_set;
	list->get =      &ArrayList_get;
	list->remove =   &ArrayList_remove;
	list->iterator = &ArrayList_iterator;
	list->size =     &ArrayList_size;
	list->clear =    &ArrayList_clear;
	list->release=   (list == NULL) ? &ArrayList_releaseSelf : &ArrayList_release;
	
	list->type = ArrayList;
	list->arrayListData.elements = NULL;
	list->arrayListData.capacity = 0;
	list->arrayListData.length = 0;
	return result;
}

/**
 * (internal)
 * Grow the elements array to at least the size of minCap
 * @return success
 * May set errno on failure
 */
static bool ArrayList_grow(List* list, const size_t minCap) {
	const size_t oldCapacity = list->arrayListData.capacity;
	if (oldCapacity >= minCap)
		return true;
	size_t newCapacity = oldCapacity * 3 / 2 + 1;
	if (newCapacity < minCap)
		newCapacity = minCap;
	void* newElems = realloc(list->arrayListData.elements, newCapacity);
	if (newElems == NULL) {
		errno = ENOMEM;
		return false;
	}
	list->arrayListData.elements = newElems;
	list->arrayListData.capacity = newCapacity;
	return true;
}
/**
 * (internal)
 * Optionally shrink the elements array
 */
static void ArrayList_shrink(List* list) {
	const size_t oldCapacity = list->arrayListData.capacity;
	const size_t length = list->arrayListData.length;
	if ((length + 10) < (oldCapacity / 2)) {
		size_t newCapacity = length * 3 / 2 + 1;
		if (newCapacity < length)
			newCapacity = length;
		void* newElems = realloc(list->arrayListData.elements, newCapacity);
		if (newElems == NULL)
			return;
		list->arrayListData.elements = newElems;
		list->arrayListData.capacity = newCapacity;
	}
}

static size_t ArrayList_add(List* list, void* value) {
	size_t index = list->arrayListData.length + 1;
	ArrayList_set(list, index, value);
	if (list->arrayListData.length != index)
		//Fail
		return -1u;
	return index;
}

static void* ArrayList_set(List* list, size_t index, void* value) {
	if (value == NULL)
		return ArrayList_remove(list, index);
	return NULL;//TODO finish
}

static void* ArrayList_get(List* list, size_t index) {
	if (index > list->arrayListData.length)
		return NULL;
	return list->arrayListData.elements[index];
}

static void* ArrayList_remove(List* list, size_t index) {
	if (index > list->arrayListData.length)
		return NULL;
	void* result = list->arrayListData.elements[index];
	list->arrayListData.elements[index] = NULL;
	if (index == list->arrayListData.length) {
		//Update length
		size_t newLength = list->arrayListData.length;
		//TODO check that no integer underflows happen here
		while (list->arrayListData.elements[newLength] == NULL) {
			if (newLength == 0)
				break;
			newLength--;
		}
		list->arrayListData.length = newLength;
		ArrayList_shrink(list);
	}
	return result;
}

static bool ArrayList_iterator_hasNext(Iterator* self) {
	return self->privI[0] < ((List*)(self->privP))->arrayListData.length;
}

static void* ArrayList_iterator_next(Iterator* self) {
	List* list = (List*) self->privP;
	const size_t nextIdx = self->privI[0];
	if (nextIdx >= list->arrayListData.length)
		//out of bounds
		return NULL;
	void* result = list->arrayListData.elements[nextIdx];
	self->privI[0]++;
	return result;
}

/**
 * (Used for iterator) removes the current element from this list.
 */
static void* ArrayList_iterator_remove(Iterator* self) {
	size_t nextIdx = self->privI[0];
	if (nextIdx == 0)
		return NULL;//Before first element
	List* list = (List*) self->privP;
	return list->remove(list, nextIdx - 1);
}

/**
 * Create an iterator that can traverse this list.
 * Internally, Iterator.privP will be a reference to this list,
 * and Iterator.privI[0] will be the next index to access.
 */
static Iterator* ArrayList_iterator(List* list) {
	Iterator* iterator = malloc(sizeof(Iterator));
	if (iterator == NULL) {
		errno = ENOMEM;
		return NULL;
	}
	
	iterator->privP = list;
	iterator->privI[0] = 0;
	iterator->hasNext = &ArrayList_iterator_hasNext;
	iterator->next = &ArrayList_iterator_next;
	
	return iterator;
}

static size_t ArrayList_size(List* list) {
	return list->arrayListData.length;
}

/**
 * Release all elements
 */
static void ArrayList_clear(List* list, Consumer* cleaner) {
	for (size_t i = 0; i < list->arrayListData.length; i++) {
		void* element = list->arrayListData.elements[i];
		if (element == NULL)
			continue;
		if (cleaner)
			cleaner->apply(cleaner->priv, element);
		list->arrayListData.elements[i] = NULL;
	}
	list->arrayListData.length = 0;
	ArrayList_shrink(list);
}

/**
 * Release all elements and internal memory
 */
static void ArrayList_release(List* list, Cleaner* cleaner) {
	ArrayList_clear(list, cleaner);
	free(list->arrayListData.elements);
	list->arrayListData.elements = NULL;
	list->arrayListData.capacity = 0;
}

/**
 * Release all elements, internal memory, and self
 */
static void ArrayList_releaseSelf(List* list, Consumer* cleaner) {
	ArrayList_clear(list, cleaner);
	free(list->arrayListData.elements);
	list->arrayListData.elements = NULL;
	list->arrayListData.capacity = 0;
	free(list);
}
