#include <stdlib.h> //malloc, free
#include <string.h> //For strcmp, memcmp

#include "../locality.h"//Mostly for UNUSED macro
#include "Comparators.h"

static void release_noop(void* p) {
	UNUSED(p);
}

static int PointerComparator_apply(void* p, const void* a, const void* b) {
	UNUSED(p);
	unsigned long aVal = (unsigned long) a;
	unsigned long bVal = (unsigned long) b;
	if (aVal > bVal)
		return 1;
	if (aVal == bVal)
		return 0;
	//assert(aVal < bVal)
	return -1;
}

Comparator* PointerComparator_init(Comparator* comparator) {
	comparator->apply = &PointerComparator_apply;
	comparator->release = &release_noop;
	comparator->priv = NULL;
	return comparator;
}

static int FixedLengthValueComparator_apply(void* p, const void* a, const void* b) {
	if (a == b)
		return 0;
	if (a == NULL)
		return -1;
	if (b == NULL)
		return 1;
	size_t length = *((size_t*)p);
	return memcmp(a, b, length);
}

Comparator* FixedLengthValueComparator_init(Comparator* comparator, size_t length) {
	if ((comparator->priv = malloc(sizeof(size_t))) == NULL)
		return NULL;
	*((size_t*)comparator->priv) = length;
	comparator->apply = &FixedLengthValueComparator_apply;
	comparator->release = &free;
	return comparator;
}

static int StringComparator_apply(void* p, const void* a, const void* b) {
	UNUSED(p);
	if (a == b)
		return 0;
	if (a == NULL)
		return -1;
	if (b == NULL)
		return 1;
	return strcmp((const char*)a, (const char*)b);
}

Comparator* StringComparator_init(Comparator* comparator) {
	comparator->apply = &StringComparator_apply;
	comparator->release = &release_noop;
	comparator->priv = NULL;
	return comparator;
}
