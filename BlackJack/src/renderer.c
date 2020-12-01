/*
 * renderer.c
 *
 *  Created on: Nov 24, 2020
 *      Author: max
 */
#include "card.h"
#include "renderer.h"
#include <stdio.h>

void show_status(const int *cash, const int *bet){
	printf("Your cash is %d, current bet is %d\n", *cash, *bet);
}

const char* request_bet_update(const int *bet){
	if ((*bet) == 0) {
		return "If you want to add bet, please enter amount (multiples of 10): ";
	} else {
		return "If you want to add bet, please enter amount (multiples of 10) or 0: ";
	}
}

void show_warning(const char *message) {
	printf(message);
}

void show_dealer_cards(const LinkedList *list) {
	const Card* first_card = (Card*) list -> head -> data;
	printf("Dealer's cards: %s %s, ????????\n", get_card_suit(first_card), get_card_rank_name(first_card));
}

void show_player_cards(const LinkedList *list) {
	printf("Player's cards: ");
	Node *node = list -> head;
	for (int i = 0; i < 2; i++) {
		if (i != 0) {
			printf(", ");
		}
		const Card* card = (Card*) node -> data;
		printf("%s %s", get_card_suit(card), get_card_rank_name(card));
		node = node -> next;
	}
	printf("\n");
}

void show_cards(const char *hand, const LinkedList *list) {
	printf("Cards of %s: ", hand);
	Node *node = list -> head;
	int counter = 0;
	while(node != NULL){
		if (counter != 0) {
			printf(", ");
		}
		const Card* card = (Card*) node -> data;
		printf("%s %s", get_card_suit(card), get_card_rank_name(card));
		node = node -> next;
		counter++;
	}
	printf("\n");
}

void show_message(const char *message) {
	printf(message);
}

