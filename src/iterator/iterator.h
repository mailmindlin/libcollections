#include <stdbool.h>
#include <stddef.h>
#include "../collections.h"
#include "../locality.h"
#ifndef __LIBCOLLECTIONS_ITERATOR_H
#define __LIBCOLLECTIONS_ITERATOR_H

typedef struct Iterator {
	bool  (*hasNext) (struct Iterator* self);
	void* (*next)    (struct Iterator* self);
	bool  (*remove)  (struct Iterator* self);
	void  (*release) (struct Iterator* self);
	void* privP;
	unsigned int privI[3];
} Iterator;

LIBCOLLECTIONS_PUBLIC Iterator* IterateOverArray(size_t offset, size_t length, void* values);

#endif