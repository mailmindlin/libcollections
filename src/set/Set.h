#include <stdbool.h>
#include <stdint.h>
#include "../collections.h"
#include "../locality.h"
#include "../iterator/iterator.h"
#include "../common/Functions.h"

#ifndef __LIBCOLLECTIONS_LIST_H
#define __LIBCOLLECTIONS_LIST_H

typedef struct Set {
	/**
	 * Add the value to the set
	 * @return success
	 */
	bool (*add) (struct Set* self, void* value) __attribute__ ((nonnull (1)));
	/**
	 * Returns whether the given value is contained in this set
	 */
	bool (*contains) (struct Set* self, void* value) __attribute__ ((nonnull (1)));
	/**
	 * Remove value from set
	 * @return value removed, else NULL if value was not contained in set
	 */
	void* (*remove) (struct Set* self, void* value) __attribute__ ((nonnull (1)));
	/**
	 * Get the size of the set (the number of elements)
	 */
	size_t (*getSize) (struct Set* self) __attribute__ ((nonnull (1)));
	/**
	 * Create an iterator over the elements of this set, in some arbitrary order.
	 * This operation SHOULD be supported, but if it is impossible for some implementations,
	 * return NULL and set errno to ENOSYS
	 */
	Iterator* (*iterator) (struct Set* self) __attribute__ ((nonnull (1)));
	/**
	 * Remove all elements of this set, optionally applying cleaner to them.
	 * If cleaner is NULL, no operation will be applied.
	 */
	void (*clear) (struct Set* self, Consumer* cleaner) __attribute__ ((nonnull (1)));
	/**
	 * Release all internal data for this set. This set SHOULD be cleared before
	 * invoking this method, else its behavior is undefined.
	 */
	void (*release) (struct Set* self) __attribute__ ((nonnull (1)));
	//Implementation-specific data. Please don't touch.
	void* priv;
} Set;

LIBCOLLECTIONS_PUBLIC Set* BitSet_create(Set* set, IntFunction* hashFn, int min, int max) __attribute__ ((nonnull (1, 2)));
LIBCOLLECTIONS_PUBLIC Set* HashSet_create(Set* set, IntFunction* hashFn) __attribute__ ((nonnull (1, 2)));
LIBCOLLECTIONS_PUBLIC Set* TreeSet_create(Set* set, Comparator* comparator) __attribute__ ((nonnull (1, 2)));


#endif
