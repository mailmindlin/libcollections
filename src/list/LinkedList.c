#include "list-priv.h"

static DoublyLinkedKeyedNode* getNodeImmediatelyLEQ(DoublyLinkedKeyedNode* head, size_t key) {
	if (head == NULL)
		return NULL;
}

size_t LinkedList_add(List* list, void* value) {
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
	head->prev->next = current;
	current->prev = head->prev;
	head->prev = current;
	current->next = head;
	current->key = current->prev->key + 1;
	list->linkedListData.numNodes++;
}

void* LinkedList_set(List* list, size_t index, void* value) {
	if (value == NULL)
		return LinkedList_remove(list, index);
	DoublyLinkedKeyedNode* head = list->linkedListData.head;
	DoublyLinkedKeyedNode* prev = getNodeImmediatelyLEQ(head, index);
	if (prev == NULL || prev->key != index) {
		//Miss (add new entry)
		DoublyLinkedKeyedNode* current = malloc(sizeof(DoublyLinkedKeyedNode));
		current->key = index;
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

void* LinkedList_get(List* list, size_t index) {
	DoublyLinkedKeyedNode* node = getNodeImmediatelyLEQ(list->linkedListData.head, index);
	if (node != NULL && node->key == index)
		return node->value;
	return NULL;
}

void* LinkedList_remove(List* list, size_t index) {
	DoublyLinkedKeyedNode* head = list->linkedListData.head;
	DoublyLinkedKeyedNode* node = getNodeImmediatelyLEQ(head, index);
	if (node != NULL && node->key == index) {
		if (--list->linkedListData.numNodes == 0)
			list->linkedListData.head = NULL;
		if (node == head) {
			
		}
	}
}

Iterator* LinkedList_iterator(List* list) {
	//TODO finish
	((void)list);
	return NULL;
}

size_t LinkedList_size(List* list) {
	return list->linkedListData.head != NULL ? list->linkedListData.head->prev->key : 0;
}

void LinkedList_clear(List* list, Cleaner* cleaner) {
	list->linkedListData.numNodes = 0;
	DoublyLinkedKeyedNode* head = list->linkedListData.head;
	if (head == NULL)
		return;
	cleaner(head->value);
	DoublyLinkedKeyedNode* current = head->next;
	free(head);
	while (current != head) {
		cleaner(current->value);
		DoublyLinkedKeyedNode* next = current->next;
		free(current);
		current = next;
	}
}

void LinkedList_release(List* list, Cleaner* cleaner) WEAKREF(LinkedList_clear);