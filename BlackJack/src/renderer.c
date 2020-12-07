/*
 * renderer.c
 *
 *  Created on: Nov 24, 2020
 *      Author: max
 */
#include "card.h"
#include "renderer.h"
#include <stdio.h>

void show_status(const unsigned int *cash, const unsigned int *bet){
	printf("Your cash is %d, current bet is %d\n", *cash, *bet);
}

void show_dealer_cards(const LinkedList_t *list) {
	const Card_t* first_card = (Card_t*) list -> head -> data;
	printf("Dealer's cards: \n%s of %s,\n ????????\n", get_card_rank_name(first_card), get_card_suit(first_card));
}

void show_cards(const char *hand, const LinkedList_t *list) {
	printf("Cards of %s: ", hand);
	Node_t *node = list -> head;
	int counter = 0;
	while(node != NULL){
		if (counter != 0) {
			printf(", ");
		}
		const Card_t* card = (Card_t*) node -> data;
		printf("\n%s of %s", get_card_rank_name(card), get_card_suit(card));
		node = node -> next;
		counter++;
	}
	printf("\n");
}

