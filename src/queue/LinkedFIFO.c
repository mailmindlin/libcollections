#include <stddef.h>	//For NULL
#include <stdlib.h>	//For malloc, free
#include <stdbool.h>
#include "queue-priv.h"

bool LinkedFIFO_push(Queue* queue, void* value) {
	SinglyLinkedNode* newHead = malloc(sizeof(SinglyLinkedNode));
	if (newHead == NULL)
		return false;
	newHead->value = value;
	newHead->next  = queue->fifo.linkedFIFOData.head;
	queue->fifo.linkedFIFOData.head = newHead;
	return true;
}

void* LinkedFIFO_peek(Queue* queue) {
	SinglyLinkedNode* head = queue->fifo.linkedFIFOData.head;
	if (head == NULL)
		return NULL;
	return head->value;
}

void* LinkedFIFO_pop (Queue* queue) {
	SinglyLinkedNode* head = queue->fifo.linkedFIFOData.head;
	if (head == NULL)
		return NULL;
	
	queue->fifo.linkedFIFOData.head = head->next;
	void* value = head->value;
	free(head);
	return value;
}

bool LinkedFIFO_empty(Queue* queue) {
	return queue->fifo.linkedFIFOData.head == NULL;
}

void LinkedFIFO_clear(Queue* queue, Consumer* cleaner) {
	while(!LinkedFIFO_empty(queue)) {
		void* elem = LinkedFIFO_pop(queue);
		if (cleaner)
			cleaner->apply(cleaner->priv, elem);
	}
}

void LinkedFIFO_release(Queue* queue, Cleaner* cleaner) WEAKREF(LinkedFIFO_clear);