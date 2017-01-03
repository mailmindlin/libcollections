#include <stddef.h>	//For NULL
#include <stdlib.h>	//For malloc, free
#include <stdbool.h>
#include "queue-priv.h"

bool LinkedLIFO_push(Queue* queue, void* value) {
	SinglyLinkedNode* newTail = malloc(sizeof(SinglyLinkedNode));
	if (newTail == NULL)
		return false;
	newTail->value = value;
	newTail->next NULL;
	
	SinglyLinkedNode* tail = queue->lifo.linkedLIFOData.head;
	//TODO finish
	return true;
}

void* LinkedLIFO_peek(Queue* queue) {
	SinglyLinkedNode* head = queue->lifo.linkedLIFOData.head;
	if (head == NULL)
		return NULL;
	return head->value;
}

void* LinkedLIFO_pop (Queue* queue) {
	SinglyLinkedNode* head = queue->lifo.linkedLIFOData.head;
	if (head == NULL)
		return NULL;
	
	queue->lifo.linkedLIFOData.head = head->next;
	void* value = head->value;
	free(head);
	return value;
}

bool LinkedLIFO_empty(Queue* queue) {
	return queue->lifo.linkedLIFOData.head == NULL;
}

void LinkedLIFO_clear(Queue* queue, Cleaner* cleaner) {
	while(!LinkedLIFO_empty(queue))
		cleaner(LinkedLIFO_pop(queue));
}