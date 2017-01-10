#ifndef __LIBCOLLECTIONS_COMMON_LINKEDNODE_H
#define __LIBCOLLECTIONS_COMMON_LINKEDNODE_H

typedef struct SinglyLinkedNode {
	struct SinglyLinkedNode* next;
	void* value;
} SinglyLinkedNode;

typedef struct SinglyLinkedKeyedNode {
	struct SinglyLinkedKeyedNode* next;
	unsigned int key;
	void* value;
} SinglyLinkedKeyedNode;

typedef struct DoublyLinkedNode {
	struct DoublyLinkedNode* next;
	struct DoublyLinkedNode* prev;
	void* value;
} DoublyLinkedNode;

typedef struct DoublyLinkedKeyedNode {
	struct DoublyLinkedKeyedNode* next;
	struct DoublyLinkedKeyedNode* prev;
	unsigned int key;
	void* value;
} DoublyLinkedKeyedNode;

#endif