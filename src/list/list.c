#include "list.h"
#include "list-priv.h"

size_t    (*add)      (struct List* list, void* value) __attribute__ ((nonnull (1)));
	void*     (*set)      (struct List* list, size_t index, void* value) __attribute__ ((nonnull (1)));
	void*     (*get)      (struct List* list, size_t index) __attribute__ ((nonnull (1)));
	void*     (*remove)   (struct List* list, size_t index) __attribute__ ((nonnull (1)));
	Iterator* (*iterator) (struct List* list) __attribute__ ((nonnull (1)));
	size_t    (*size)     (struct List* list) __attribute__ ((nonnull (1)));
	void      (*clear)    (struct List* list, Cleaner* cleaner) __attribute__ ((nonnull (1)));
	void      (*release)  (struct List* list, Cleaner* cleaner) __attribute__ ((nonnull (1)));
	
bool InitList(List* list, CollectionType type) {
	list->type = type;
	switch (type) {
		case LinkedList:
			list->add =      &LinkedList_add;
			list->set =      &LinkedList_set;
			list->get =      &LinkedList_get;
			list->remove =   &LinkedList_remove;
			list->iterator = &LinkedList_iterator;
			list->size =     &LinkedList_size;
			list->clear =    &LinkedList_clear;
			list->release=   &LinkedList_release;
			list->linkedListData.head = NULL;
			list->linkedListData.numNodes = 0;
			return true;
		case ArrayList:
			list->add =      &ArrayList_add;
			list->set =      &ArrayList_set;
			list->get =      &ArrayList_get;
			list->remove =   &ArrayList_remove;
			list->iterator = &ArrayList_iterator;
			list->size =     &ArrayList_size;
			list->clear =    &ArrayList_clear;
			list->release=   &ArrayList_release;
			list->arrayListData.elements = NULL;
			list->arrayListData.capacity = 0;
			list->arrayListData.length = 0;
			return true;
		default:
			errno = EINVAL;
			return false;
	}
}