#include <stdlib.h> //For malloc, free
#include <errno.h> //For, well, errno & values
#include "list.h"

/*
 * File contains implementation of a doubly linked list.
 */

//LinkedList internal methods
static DoublyLinkedKeyedNode* getNodeImmediatelyLEQ(DoublyLinkedKeyedNode* head, size_t key);

//LinkedList methods
static size_t LinkedList_add     (List* list, void* value) __attribute__ ((nonnull (1)));
static void*  LinkedList_set     (List* list, size_t index, void* value) __attribute__ ((nonnull (1)));
static void*  LinkedList_get     (List* list, size_t index) __attribute__ ((nonnull (1)));
static void*  LinkedList_remove  (List* list, size_t index) __attribute__ ((nonnull (1)));
static Iterator* LinkedList_iterator(List* list) __attribute__ ((nonnull (1)));
static size_t LinkedList_size    (List* list) __attribute__ ((nonnull (1)));
static void   LinkedList_clear   (List* list, Consumer* cleaner) __attribute__ ((nonnull (1)));
static void   LinkedList_release (List* list, Consumer* cleaner) __attribute__ ((nonnull (1)));


/**
 * Initialize LinkedList
 */
List* LinkedList_create(List* list) {
	List* result = list;
	if (result == NULL) {
		if ((result = malloc(sizeof(List))) == NULL) {
			errno = ENOMEM;
			return NULL;
		}
	}
	
	result->add =      &LinkedList_add;
	result->set =      &LinkedList_set;
	result->get =      &LinkedList_get;
	result->remove =   &LinkedList_remove;
	result->iterator = &LinkedList_iterator;
	result->size =     &LinkedList_size;
	result->clear =    &LinkedList_clear;
	//Release list memory only if we allocated it
	result->release =  (list == NULL) ? &LinkedList_release : &LinkedList_clear;
	
	list->type = LinkedList;
	result->linkedListData.head = NULL;
	result->linkedListData.numNodes = 0;
	return result;
}

/**
 * (internal)
 * Get either the node with the given key, or the one immediately before where it should be
 * Returns null if no such node exists (no nodes exist)
 */
static DoublyLinkedKeyedNode* getNodeImmediatelyLEQ(DoublyLinkedKeyedNode* head, size_t key) {
	if (head == NULL)
		return NULL;
	if (head->key > key)
		return head->prev;
	DoublyLinkedKeyedNode* current = head;
	while ((current->key < key) && (current->next != head))
		current = current->next;
	return current;
}

static size_t LinkedList_add(List* list, void* value) {
	if (value == NULL)
		return list->linkedListData.numNodes;
	DoublyLinkedKeyedNode* current = malloc(sizeof(DoublyLinkedKeyedNode));
	if (current == NULL)
		return -1u;
	current->value = value;
	
	if (list->linkedListData.numNodes == 0) {
		current->key = 0;
		current->next = current;
		current->prev = current;
		list->linkedListData.head = current;
		list->linkedListData.numNodes = 1;
		return 0;
	}
	DoublyLinkedKeyedNode* head = list->linkedListData.head;
	//Insert to the left of the head
	head->prev->next = current;
	current->prev = head->prev;
	head->prev = current;
	current->next = head;
	current->key = current->prev->key + 1;
	list->linkedListData.numNodes++;
	return current->key;
}

static void* LinkedList_set(List* list, size_t index, void* value) {
	if (value == NULL)
		return LinkedList_remove(list, index);
	DoublyLinkedKeyedNode* head = list->linkedListData.head;
	DoublyLinkedKeyedNode* prev = getNodeImmediatelyLEQ(head, index);
	if (prev == NULL || prev->key != index) {
		//Miss (add new entry)
		DoublyLinkedKeyedNode* current = malloc(sizeof(DoublyLinkedKeyedNode));
		current->key = (unsigned int) index;
		current->value = value;
		if (prev == NULL || index < head->key) {
			if (prev == NULL) {
				//No head (link to self)
				current->next = current;
				current->prev = current;
			} else {
				//Insert before head
				current->next = head;
				current->prev = head->prev;
			}
			//Update head reference
			list->linkedListData.head = current;
		} else {
			//Insert after prev
			current->prev = prev;
			current->next = prev->next;
		}
		//Finish updating pointers
		current->prev->next = current;
		current->next->prev = current;
		return NULL;
	} else {
		// Hit (replace the entry)
		void* result = prev->value;
		prev->value = value;
		return result;
	}
}

static void* LinkedList_get(List* list, size_t index) {
	DoublyLinkedKeyedNode* node = getNodeImmediatelyLEQ(list->linkedListData.head, index);
	if (node != NULL && node->key == index)
		return node->value;
	return NULL;
}

static void* LinkedList_remove(List* list, size_t index) {
	DoublyLinkedKeyedNode* head = list->linkedListData.head;
	DoublyLinkedKeyedNode* node = getNodeImmediatelyLEQ(head, index);
	if (node == NULL || node->key != index)
		//Didn't find it
		return NULL;
	//Handle head pointer updating
	if (node == head)
		list->linkedListData.head = node->next;
	if (--list->linkedListData.numNodes == 0)
		list->linkedListData.head = NULL;
	//Unlink the node
	node->next->prev = node->prev;
	node->prev->next = node->next;
	void* result = node->value;
	free(node);
	return result;
}

static Iterator* LinkedList_iterator(List* list) {
	UNUSED(list);
	//TODO finish
	return NULL;
}

static size_t LinkedList_size(List* list) {
	return list->linkedListData.head != NULL ? list->linkedListData.head->prev->key : 0;
}

static void LinkedList_clear(List* list, Consumer* cleaner) {
	list->linkedListData.numNodes = 0;
	DoublyLinkedKeyedNode* head = list->linkedListData.head;
	if (head == NULL)
		return;
	if (cleaner)
		cleaner->apply(cleaner->priv, head->value);
	DoublyLinkedKeyedNode* current = head->next;
	free(head);
	while (current != head) {
		if (cleaner)
			cleaner->apply(cleaner->priv, current->value);
		DoublyLinkedKeyedNode* next = current->next;
		free(current);
		current = next;
	}
}

static void LinkedList_release(List* list, Consumer* cleaner) {
	LinkedList_clear(list, cleaner);
	free(list);
}