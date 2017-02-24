#include <errno.h>
#include <stdlib.h>
#include "iterator.h"

static bool Iterator_removeFail(Iterator* iterator) __attribute__ ((nonnull (1)));
static void Iterator_releaseSelf(Iterator* iterator) __attribute__ ((nonnull (1)));
/**
 * ArrayIterator:
 * privP = pointer to array
 * privI[0] = next index
 * privI[1] = array length
 */
static bool Iterator_removeFail(Iterator* iterator) {
	errno = ENOTSUP;
	return false;
}

static void Iterator_releaseSelf(Iterator* iterator) {
	free(iterator);
}

static bool ArrayIterator_hasNext(Iterator* iterator) {
	const unsigned long nextIdx = iterator->privI[0];
	const unsigned long arrayLen = iterator->privI[1];
	return nextIdx > arrayLen;
}

static void* ArrayIterator_next(Iterator* iterator) {
	const unsigned long currentIdx = iterator->privI[0];
	const unsigned long arrayLen = iterator->privI[1];
	if (currentIdx > arrayLen)
		return NULL;
	iterator->privI[0]++;
	return ((void**)iterator->privP)[currentIdx];
}

Iterator* IterateOverArray(size_t offset, size_t length, void* values) {
	Iterator* iterator = malloc(sizeof(Iterator));
	if (iterator == NULL) {
		errno = ENOMEM;
		return NULL;
	}
	iterator->privI[0] = offset;
	iterator->privI[1] = length;
	iterator->privP = values;
	iterator->remove = &Iterator_removeFail;
	iterator->hasNext = &ArrayIterator_hasNext;
	iterator->next = &ArrayIterator_next;
	iterator->release = &Iterator_releaseSelf;
	return iterator;
}