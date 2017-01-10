#include <stdbool.h>
#include "../collections.h"
#include "../locality.h"
#ifndef __LIBCOLLECTIONS_ITERATOR_H
#define __LIBCOLLECTIONS_ITERATOR_H

typedef struct Iterator {
	bool  (*hasNext) (Iterator* self);
	void* (*next)    (Iterator* self);
	bool  (*remove)  (Iterator* self);
	void  (*release) (Iterator* self);
	void* privP;
	unsigned int privI[3];
} Iterator;

LIBCOLLECTIONS_PUBLIC Iterator* IterateOverArray(size_t offset, size_t length, void* values);

#endif