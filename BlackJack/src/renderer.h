/*
 * renderer.h
 *
 *  Created on: Nov 24, 2020
 *      Author: max
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include "linked_list.h"

void show_status(const unsigned int *cash, const unsigned int *bet);

void show_dealer_cards(const LinkedList_t *list);

void show_cards(const char *hand, const LinkedList_t *list);

#endif /* SRC_RENDERER_H_ */
