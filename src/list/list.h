#include <stdbool.h>
#include "../collections.h"
#include "../locality.h"
#include "../iterator/iterator.h"
#include "../common/LinkedNode.h"
#ifndef __LIBCOLLECTIONS_LIST_H
#define __LIBCOLLECTIONS_LIST_H

typedef struct List {
	bool  (*add) (struct List* list, size_t index, void* value);
	void* (*get) (struct List* list, size_t index);
	void* (*remove) (struct List* list, size_t index);
	Iterator* (*iterator) (struct List* list);
	size_t (*size) (struct List* list);
	void  (*clear) (struct List* list, Cleaner* cleaner);
	void  (*release) (struct List* list, Cleaner* cleaner);
	CollectionType type;
	//Implementation-specific data. Please don't touch.
	union {
		struct {
			void* elements[];
			size_t capacity;
			size_t length;
		} arrayListData;
		struct {
			DoublyLinkedNode* head;
		} linkedListData;
	};
} List;

#endif