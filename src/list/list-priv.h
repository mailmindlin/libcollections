#include <stdbool.h>
#include "../collections.h"
#include "../locality.h"
#include "list.h"

#ifndef __LIBCOLLECTIONS_LIST_PRIV_H
#define __LIBCOLLECTIONS_LIST_PRIV_H

// List iterator implementation methods
LIBCOLLECTIONS_LOCAL Iterator* ListIterator_create(List* list) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL bool   ListIterator_hasNext(Iterator* self) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL void*  ListIterator_remove(Iterator* self) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL void*  ListIterator_next(Iterator* self) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL void   ListIterator_release(Iterator* self) __attribute__ ((nonnull (1)));
#endif