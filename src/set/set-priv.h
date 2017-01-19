#include "Set.h"
#include "../locality.h"

#ifndef __LIBCOLLECTIONS_SET_PRIV_H
#define __LIBCOLLECTIONS_SET_PRIV_H

LIBCOLLECTIONS_LOCAL bool HashSet_add(Set* self, void* value) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL bool HashSet_contains(Set* self, void* value) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL void* HashSet_remove(Set* self, void* value) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL size_t HashSet_getSize(Set* self) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL Iterator* HashSet_iterator(Set* self) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL void HashSet_clear(Set* self, Consumer* cleaner) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL void HashSet_release(Set* self) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL void HashSet_releaseSelf(Set* self) __attribute__ ((nonnull (1)));

#endif //__LIBCOLLECTIONS_SET_PRIV_H