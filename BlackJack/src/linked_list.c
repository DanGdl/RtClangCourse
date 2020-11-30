/*
 * linked_list.c
 *
 *  Created on: Nov 24, 2020
 *      Author: max
 */
#include "linked_list.h"
#include <stdlib.h>
#include "errors.h"

LinkedList* createList() {
	LinkedList *l = (LinkedList*) malloc(sizeof(LinkedList));
	l -> head = NULL;
	l -> last = NULL;
	return l;
}

int size(const LinkedList *list){
	if (list == NULL) {
		log_error("Size of List that is NULL!!");
	}
	int size = 0;
	Node *node = list -> head;
	while (node != NULL) {
		size++;
		node = node -> next;
	}
	return size;
}

void add(LinkedList *list, void *card) {
	if (list == NULL) {
		log_error("Add to List that is NULL!!");
	}
	if (card == NULL) {
		log_error("Adding card that is NULL!!");
	}
	if (list -> head == NULL) {
		list -> head = (Node*) malloc(sizeof(Node));
		list -> last = list -> head;

		Node *node = list -> head;

		node -> next = NULL;
		node -> previous = NULL;
		node -> data = card;
	} else {
		Node *node = list -> head;
		while (node -> next != NULL) {
			node = node -> next;
		}

		node -> next = (Node*) malloc(sizeof(Node));
		list -> last = node -> next;

		node -> next -> next = NULL;
		node -> next -> previous = node;
		node -> next -> data = card;
	}
}


void clear(LinkedList *list) {
	if (list == NULL) {
		log_error("Clear List that is NULL!!");
	}
	Node *node = list -> last;
	while (node != NULL) {
		Node *tmp = node;
		node = node -> previous;

		free(tmp -> data);
		free(tmp);

		if (node != NULL) {
			node -> next = NULL;
		}
		list -> last = node;
	}
	list -> head = NULL;
	list -> last = NULL;
}

void* remove_last(LinkedList *list) {
	if (list == NULL) {
		log_error("Remove last from List that is NULL!!");
	}
	Node * node = list -> last;
	void *card = node -> data;
	list -> last = node -> previous;
	if (list -> last == NULL) {
		list -> head = NULL;
	} else {
		list -> last -> next = NULL;
	}
	node -> data = NULL;
	free(node);
	return card;
}
