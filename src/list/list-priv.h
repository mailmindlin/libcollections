#include <stdbool.h>
#include "../collections.h"
#include "../locality.h"
#include "list.h"

#ifndef __LIBCOLLECTIONS_LIST_PRIV_H
#define __LIBCOLLECTIONS_LIST_PRIV_H

//LinkedList methods
LIBCOLLECTIONS_LOCAL size_t LinkedList_add     (List* list, void* value) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL void*  LinkedList_set     (List* list, size_t index, void* value) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL void*  LinkedList_get     (List* list, size_t index) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL void*  LinkedList_remove  (List* list, size_t index) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL Iterator* LinkedList_iterator(List* list) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL size_t LinkedList_size    (List* list) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL void   LinkedList_clear   (List* list, Cleaner* cleaner) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL void   LinkedList_release (List* list, Cleaner* cleaner) __attribute__ ((nonnull (1)));

//ArrayList methods
LIBCOLLECTIONS_LOCAL size_t ArrayList_add     (List* list, void* value) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL void*  ArrayList_set     (List* list, size_t index, void* value) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL void*  ArrayList_get     (List* list, size_t index) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL void*  ArrayList_remove  (List* list, size_t index) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL Iterator* ArrayList_iterator(List* list) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL size_t ArrayList_size    (List* list) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL void   ArrayList_clear   (List* list, Cleaner* cleaner) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL void   ArrayList_release (List* list, Cleaner* cleaner) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL void   ArrayList_releaseSelf(List* list, Cleaner* cleaner) __attribute__ ((nonnull (1)));

// List iterator implementation methods
LIBCOLLECTIONS_LOCAL Iterator* ListIterator_create(List* list) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL bool   ListIterator_hasNext(Iterator* self) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL void*  ListIterator_remove(Iterator* self) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL void*  ListIterator_next(Iterator* self) __attribute__ ((nonnull (1)));
LIBCOLLECTIONS_LOCAL void   ListIterator_release(Iterator* self) __attribute__ ((nonnull (1)));
#endif