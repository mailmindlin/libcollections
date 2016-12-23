#include <stddef.h>	//For NULL
#include <stdlib.h>	//For malloc, free
#include "queue.h"
#include "queue-priv.h"

bool LinkedQueue_push(Queue* queue, void* value) {
	SinglyLinkedNode* node = malloc(sizeof(SinglyLinkedNode));
	if (node == NULL)
		return false;
	node->value = value;
	node->next = NULL;
	//Note: tail is not guarunteed to be null if the queue is empty, but the head is
	if (queue->linkedQueueData.head == NULL) {
		queue->linkedQueueData.head = node;
	} else {
		queue->linkedQueueData.tail->next = node;
	}
	queue->linkedQueueData.tail = node;
}

void* LinkedQueue_peek(Queue* queue) {
	SinglyLinkedNode* head = queue->linkedQueueData.head;
	if (head == NULL)
		return NULL;
	return head->value;
}

void* LinkedQueue_pop(Queue* queue) {
	SinglyLinkedNode* head = queue->linkedQueueData.head;
	if (head == NULL)
		return NULL;
	queue->linkedQueueData.head = head->next;
	void* result = head->value;
	free(head);
	return result;
}

bool LinkedQueue_empty(Queue* queue) {
	return queue->linkedQueueData.head == NULL;
}

void LinkedQueue_clear(Queue* queue, Cleaner* cleaner) {
	SinglyLinkedNode* head = queue->linkedQueueData.head;
	while (head != NULL) {
		cleaner(head->value);
		SinglyLinkedNode* next = head->next;
		free(head);
		head = head->next;
	}
}