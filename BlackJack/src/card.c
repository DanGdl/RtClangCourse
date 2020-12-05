/*
 * card.c
 *
 *  Created on: Nov 24, 2020
 *      Author: max
 */

#include "card.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Card_t* new_card(int rank, int suit) {
	Card_t *c = (Card_t*) malloc(sizeof(Card_t));
	c -> rank = rank;
	c -> suit = suit;
	return c;
}

char* get_card_suit(const Card_t *card){
	if (card == NULL) {
		log_error_exit("Get suit of Card that is NULL!!");
	}
	switch(card -> suit) {
		case SPADES: return "Spades";
		case DIAMONDS: return "Diamonds";
		case HEARTS: return "Hearts";
		case CLUBS: return "Clubs";
		default: {
			log_error_exit("Unknown suit");
			return "";
		}
	}
}

char* get_card_rank_name(const Card_t *card) {
	if (card == NULL) {
		log_error_exit("Get rank of Card that is NULL!!");
		return "";
	}

	char *buff = (char*) malloc(5 * sizeof(char));
	memset(buff, 0, 5 * sizeof(char));

	if (card -> rank == 0) {
		sprintf(buff, "Ace");
	}
	else if (card -> rank >= 1 && card->rank < 10) {
		sprintf(buff, "%d", card -> rank + 1);
	}
	else if (card -> rank >= 10 && card -> rank < 13) {
	    switch(card -> rank) {
	        case 10: {
	        	sprintf(buff, "Jack");
	        	break;
	        }
	        case 11:  {
	        	sprintf(buff, "Queen");
	        	break;
	        }
	        case 12:  {
	        	sprintf(buff, "King");
	        	break;
	        }
	        default:{
	        	free(buff);
	        	log_error_exit("Unknown rank. NOK!");
	        }
	    }
	} else {
		free(buff);
	    log_error_exit("Unknown rank");
	}
	return buff;
}

int get_card_value(const Card_t *card) {
	if (card == NULL) {
		log_error_exit("Get value of Card that is NULL!!");
		return 0;
	}
	else if (card -> rank >= 0 && card->rank < 11) {
		return card -> rank + 1;
	}
	else if (card -> rank >=11 && card -> rank < 13) {
		return 10;
	} else {
		log_error_exit("Unknown rank");
		return 0;
	}
}

void print_card(const Card_t *card) {
	if (card == NULL) {
		log_error_exit("Print Card that is NULL!!");
	}
	char *name = get_card_rank_name(card);
	printf("%s %s", get_card_suit(card), name);
	free(name);
}

int is_ace(const Card_t *card) {
	if (card == NULL) {
		log_error_exit("IsAce on Card that is NULL!!");
	}
	return card -> rank == 0;
}
