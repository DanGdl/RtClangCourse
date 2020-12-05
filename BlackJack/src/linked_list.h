/*
 * linked_list.h
 *
 *  Created on: Nov 23, 2020
 *      Author: max
 */
#ifndef LINKED_LIST
#define LINKED_LIST

#include "card.h"

typedef struct Node_t {
	struct Node_t *next;
	struct Node_t *previous;
	Card_t *data;
} Node_t;

typedef struct LinkedList_t {
	Node_t *head;
	Node_t *last;
	unsigned int size;
} LinkedList_t;

LinkedList_t* create_list();

int size(const LinkedList_t *list);

void add(LinkedList_t *list, Card_t *card);

void clear(LinkedList_t *list);

Card_t* remove_last(LinkedList_t *list);

#endif
