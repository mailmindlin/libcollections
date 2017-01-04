#include <stdbool.h>
#include "../collections.h"
#include "../locality.h"
#ifndef __LIBCOLLECTIONS_ITERATOR_H
#define __LIBCOLLECTIONS_ITERATOR_H

typedef struct Iterator {
	bool  (*hasNext) (Iterator* iterator);
	void* (*next) (Iterator* iterator);
	bool  (*remove) (Iterator* iterator);
} Iterator;

#endif