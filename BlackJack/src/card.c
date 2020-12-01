/*
 * card.c
 *
 *  Created on: Nov 24, 2020
 *      Author: max
 */

#include "card.h"
#include "errors.h"
#include <stdio.h>

Card* newCard(int rank, int suit) {
	Card *c = (Card*) malloc(sizeof(Card));
	c -> rank = rank;
	c -> suit = suit;
	return c;
}

char* get_card_suit(const Card *card){
	if (card == NULL) {
		log_error("Get suit of Card that is NULL!!");
	}
	switch(card -> suit) {
		case SPADES: return "Spades";
		case DIAMONDS: return "Diamonds";
		case HEARTS: return "Hearts";
		case CLUBS: return "Clubs";
		default: {
			log_error("Unknown suit");
			return "";
		}
	}
}

char* get_card_rank_name(const Card *card) {
	if (card == NULL) {
		log_error("Get rank of Card that is NULL!!");
	}
	switch(card -> rank) {
		case 0: return "Ace";
		case 1: return "2";
		case 2: return "3";
		case 3: return "4";
		case 4: return "5";
		case 5: return "6";
		case 6: return "7";
		case 7: return "8";
		case 8: return "9";
		case 9: return "10";
		case 10: return "Jack";
		case 11: return "Queen";
		case 12: return "King";
		default: {
			log_error("Unknown rank");
			return "";
		}
	}
}

int get_card_value(const Card *card) {
	if (card == NULL) {
		log_error("Get value of Card that is NULL!!");
	}
	switch(card -> rank) {
		case 0: return 1; // Ace
		case 1: return 2;
		case 2: return 3;
		case 3: return 4;
		case 4: return 5;
		case 5: return 6;
		case 6: return 7;
		case 7: return 8;
		case 8: return 9;
		case 9: return 10;
		case 10: // Jack
		case 11: // Queen
		case 12: return 10; // King
		default: {
			log_error("Unknown rank");
			return "";
		}
	}
}

void print_card(Card *card) {
	if (card == NULL) {
		log_error("Print Card that is NULL!!");
	}
	const char *rank = get_card_rank_name(card);
	const char *suit = get_card_suit(card);
	printf("%s %s", suit, rank);
}

int is_ace(const Card *card) {
	if (card == NULL) {
		log_error("IsAce on Card that is NULL!!");
	}
	return card -> rank == 0;
}
