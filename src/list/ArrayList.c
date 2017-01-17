#include <errno.h> //For errno
#include <stdlib.h>//For realloc, free
#include <stddef.h>//For size_t
#include "list-priv.h"

/**
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
 * Optionally shrink the elements array
 */
static void ArrayList_shrink(List* list) {
	const size_t oldCapacity = list->arrayListData.capacity;
	const size_t length = list->arrayListData.length;
	
}

size_t ArrayList_add(List* list, void* value) {
	size_t index = list->arrayListData.length + 1;
	ArrayList_set(list, index, value);
	if (list->arrayListData.length != index)
		//Fail
		return -1u;
	return index;
}

void* ArrayList_set(List* list, size_t index, void* value) {
	if (value == NULL)
		return ArrayList_remove(list, index);
	
}

void* ArrayList_get(List* list, size_t index) {
	if (index > list->arrayListData.length)
		return NULL;
	return list->arrayListData.elements[index];
}

void* ArrayList_remove(List* list, size_t index) {
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

static ArrayList_iterator_hasNext(Iterator* self) {
	return self->privI[0] < ((List*)(self->privP))->arrayListData.length;
}

static void* ArrayList_iterator_next(Iterator* self) {
	List* list = (List*) self->privP;
	
}

Iterator* ArrayList_iterator(List* list) {
	Iterator* iterator = malloc(sizeof(Iterator));
	if (iterator == NULL) {
		errno = ENOMEM;
		return NULL;
	}
	
	iterator->hasNext = &ArrayList_iterator_hasNext;
	iterator->next = &ArrayList_iterator_next;
	
	return iterator;
}

size_t ArrayList_size(List* list) {
	return list->arrayListData.length;
}

void ArrayList_clear(List* list, Cleaner* cleaner) {
	for (size_t i = 0; i < list->arrayListData.length; i++) {
		void* element = list->arrayListData.elements[i];
		if (element == NULL)
			continue;
		cleaner(element);
		list->arrayListData.elements[i] = NULL;
	}
	list->arrayListData.length = 0;
	ArrayList_shrink(list);
}

void ArrayList_release(List* list, Cleaner* cleaner) {
	ArrayList_clear(list, cleaner);
	free(list->arrayListData.elements);
	list->arrayListData.elements = NULL;
	list->arrayListData.capacity = 0;
}