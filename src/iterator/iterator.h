#include <stdbool.h>
#include <stddef.h>
#include "../collections.h"
#include "../locality.h"
#ifndef __LIBCOLLECTIONS_ITERATOR_H
#define __LIBCOLLECTIONS_ITERATOR_H

typedef struct Iterator {
	bool  (*hasNext) (struct Iterator* self) __attribute__ ((nonnull (1)));
	void* (*next)    (struct Iterator* self) __attribute__ ((nonnull (1)));
	bool  (*remove)  (struct Iterator* self) __attribute__ ((nonnull (1)));
	void  (*release) (struct Iterator* self) __attribute__ ((nonnull (1)));
	void* privP;
	unsigned long privI[3];
} Iterator;

LIBCOLLECTIONS_PUBLIC Iterator* IterateOverArray(size_t offset, size_t length, void* values) __attribute__ ((nonnull (3), malloc, warn_unused_result));

#endif