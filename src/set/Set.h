#include <stdbool.h>
#include <stdint.h>
#include "../collections.h"
#include "../locality.h"
#include "../iterator/iterator.h"
#ifndef __LIBCOLLECTIONS_LIST_H
#define __LIBCOLLECTIONS_LIST_H

typedef struct Set {
	/**
	 * Add the value to the set
	 * @return success
	 */
	bool (*add) (struct Set* self, void* value);
	/**
	 * Returns whether the given value is contained in this set
	 */
	bool (*contains) (struct Set* self, void* value);
	/**
	 * Remove value from set
	 * @return value removed, else NULL if value was not contained in set
	 */
	void* (*remove) (struct Set* self, void* value);
	/**
	 * Get the size of the set (the number of elements)
	 */
	size_t (*getSize) (struct Set* self);
	/**
	 * Create an iterator over the elements of this set, in some arbitrary order.
	 * This operation SHOULD be supported, but if it is impossible for some implementations,
	 * return NULL and set errno to ENOSYS
	 */
	Iterator* (*iterator) (struct Set* self);
	/**
	 * Remove all elements of this set, optionally applying cleaner to them.
	 * If cleaner is NULL, no operation will be applied.
	 */
	void (*clear) (struct Set* self, Cleaner* cleaner);
	/**
	 * Release all internal data for this set. This set SHOULD be cleared before
	 * invoking this method, else its behavior is undefined.
	 */
	void (*release) (struct Set* self);
	//Implementation-specific data. Please don't touch.
	void* priv;
} Set;

/**
 * Initialize set
 * Data types are:
 *  - Hash sets:
 *    Note: if no hash function is supplied, the pointer itself will be used (identity).
 *    - BitSet (3, hash function, min value, max value)
 *    - HashSet (1, hash function)
 *  - Comparison sets:
 *    - TreeSet (1, comparator)
 * Returns set, else NULL on error (may set errno)
 */
LIBCOLLECTIONS_PUBLIC Set* InitSet(Set* set, CollectionType type, size_t numArgs, ...) __attribute__ ((nonnull (1)));

#endif
