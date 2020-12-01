/*
 * linked_list.h
 *
 *  Created on: Nov 23, 2020
 *      Author: max
 */
#ifndef LINKED_LIST
#define LINKED_LIST

typedef struct Node {
	struct Node *next;
	struct Node *previous;
	void *data;
} Node;

typedef struct LinkedList {
	Node *head;
	Node *last;
} LinkedList;

LinkedList* createList();

int size(const LinkedList *list);

void add(LinkedList *list, void *card);

void clear(LinkedList *list);

void* remove_last(LinkedList *list);

#endif
