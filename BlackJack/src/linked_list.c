/*
 * linked_list.c
 *
 *  Created on: Nov 24, 2020
 *      Author: max
 */
#include "linked_list.h"
#include <stdlib.h>
#include "errors.h"

LinkedList_t* create_list() {
	LinkedList_t *l = (LinkedList_t*) malloc(sizeof(LinkedList_t));
	l -> head = NULL;
	l -> last = NULL;
	l -> size = 0;
	return l;
}

int size(const LinkedList_t *list){
	if (list == NULL) {
		log_error_exit("Size of List that is NULL!!");
	}
	return list -> size;
}

Node_t* create_node(Card_t *data, Node_t *previous) {
	Node_t *node = (Node_t*) malloc(sizeof(Node_t));
	node -> next = NULL;
	node -> previous = previous;
	node -> data = data;
	return node;
}

void add(LinkedList_t *list, Card_t *card) {
	if (list == NULL) {
		log_error_exit("Add to List that is NULL!!");
	}
	if (card == NULL) {
		log_error_exit("Adding card that is NULL!!");
	}
	if (list -> head == NULL) {
		Node_t *node = create_node(card, NULL);

		list -> head = node;
		list -> last = node;
	} else {
		Node_t *node = create_node(card, list -> last);

		list -> last -> next = node;
		list -> last = node;
	}
	list -> size += 1;
}


void clear(LinkedList_t *list) {
	if (list == NULL) {
		log_error_exit("Clear List that is NULL!!");
	}
	Node_t *node = list -> last;
	while (node != NULL) {
		Node_t *tmp = node;
		node = node -> previous;

		if (tmp != NULL) {
			free(tmp);
		}

		if (node != NULL) {
			node -> next = NULL;
		}
		list -> last = node;
	}
	list -> head = NULL;
	list -> last = NULL;
	list -> size = 0;
}

Card_t* remove_last(LinkedList_t *list) {
	if (list == NULL) {
		log_error_exit("Remove last from List that is NULL!!");
	}
	Node_t * node = list -> last;
	void *card = node -> data;
	list -> last = node -> previous;
	if (list -> last == NULL) {
		list -> head = NULL;
	} else {
		list -> last -> next = NULL;
	}
	node -> data = NULL;
	free(node);
	list -> size -= 1;
	return card;
}

