#include <stdbool.h>
#include "../collections.h"
#include "../locality.h"
#include "list.h"

#ifndef __LIBCOLLECTIONS_LIST_PRIV_H
#define __LIBCOLLECTIONS_LIST_PRIV_H

LIBCOLLECTIONS_LOCAL size_t LinkedList_add     (List* list, void* value) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL void*  LinkedList_set     (List* list, size_t index, void* value) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL void*  LinkedList_get     (List* list, size_t index) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL void*  LinkedList_remove  (List* list, size_t index) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL Iterator* LinkedList_iterator(List* list) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL size_t LinkedList_size    (List* list) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL void   LinkedList_clear   (List* list, Cleaner* cleaner) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL void   LinkedList_release (List* list, Cleaner* cleaner) __attribute__ ((nonnull (1)));

#endif