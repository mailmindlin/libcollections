#include <stdbool.h>//For bool
#include <stddef.h>//For size_t
#include "../collections.h"
#include "../locality.h"
#include "../iterator/iterator.h"
#include "../common/LinkedNode.h"
#ifndef __LIBCOLLECTIONS_LIST_H
#define __LIBCOLLECTIONS_LIST_H

typedef struct List {
	size_t    (*add)      (struct List* list, void* value) __attribute__ ((nonnull (1)));
	void*     (*set)      (struct List* list, size_t index, void* value) __attribute__ ((nonnull (1)));
	void*     (*get)      (struct List* list, size_t index) __attribute__ ((nonnull (1)));
	void*     (*remove)   (struct List* list, size_t index) __attribute__ ((nonnull (1)));
	Iterator* (*iterator) (struct List* list) __attribute__ ((nonnull (1)));
	size_t    (*size)     (struct List* list) __attribute__ ((nonnull (1)));
	void      (*clear)    (struct List* list, Consumer* cleaner) __attribute__ ((nonnull (1)));
	void      (*release)  (struct List* list, Consumer* cleaner) __attribute__ ((nonnull (1)));
	CollectionType type;
	//Implementation-specific data. Please don't touch.
	union {
		struct {
			void** elements;
			size_t capacity;
			size_t length;
		} arrayListData;
		struct {
			DoublyLinkedKeyedNode* head;
			size_t numNodes;
		} linkedListData;
	};
} List;

LIBCOLLECTIONS_PUBLIC bool InitList(List* list, CollectionType type);

#endif