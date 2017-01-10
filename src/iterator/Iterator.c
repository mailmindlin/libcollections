#include <errno.h>
#include "Iterator.h"

/**
 * ArrayIterator:
 * privP = pointer to array
 * privI[0] = next index
 * privI[1] = array length
 */
LIBCOLLECTIONS_LOCAL void* Iterator_removeFail(Iterator* iterator) {
	errno = ENOTSUP;
	return NULL;
}

LIBCOLLECTIONS_LOCAL void Iterator_releaseSelf(Iterator* iterator) {
	free(iterator);
}

static bool ArrayIterator_hasNext(Iterator* iterator) {
	const unsigned int nextIdx = iterator->privI[0];
	const unsigned int arrayLen = iterator->privI[1];
	return nextIdx > arrayLen;
}

static void* ArrayIterator_next(Iterator* iterator) {
	const unsigned int currentIdx = iterator->privI[0];
	const unsigned int arrayLen = iterator->privI[1];
	if (currentIdx > arrayLen)
		return NULL;
	iterator->privI[0]++;
	return iterator->privP[currentIdx];
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