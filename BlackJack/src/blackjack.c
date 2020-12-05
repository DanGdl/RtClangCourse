/*
 * blackjack.c
 *
 *  Created on: Nov 24, 2020
 *      Author: max
 */

#include "blackjack.h"
#include "errors.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int calculate_points(const LinkedList_t *list, bool is_dealer) {
	int points = 0;
	int aces = 0;
	Node_t *node = list -> last;
	while (node != NULL) {
		const Card_t *card = (Card_t*) node -> data;
		points += get_card_value(card);
		if (is_ace(card)) {
			aces += 1;
		}
		node = node -> previous;
	}
	if (aces > 0) {
		if (is_dealer && aces == 2 && points + 20 <= 22) {
			points += 22;
		} else if (points + 10 <= 21) {
			points += 10;
		}
	}
	return points;
}

Card_t* get_random_card(Card_t **deck) {
	do {
		const int position = rand() % DECK_SIZE;
		if (deck[position] == NULL) {
			continue;
		}
		Card_t *card = deck[position];
		deck[position] = NULL;
		if (card == NULL) {
			log_error_exit("Random card is NULL!!");
		}
		return card;
	} while(1);
}

void fill_deck(Card_t **deck) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 13; j++) {
			const int idx = i * 13 + j;
			if (deck[idx] == NULL) {
				deck[idx] = new_card(j, i);
			}
		}
	}
}

void update_bet(unsigned int *bet, const unsigned int *cash) {
	int bet_change = -1;
	do {
		printf((*bet) == 0 ? "If you want to add bet, please enter amount (multiples of 10): "
				: "If you want to add bet, please enter amount (multiples of 10) or 0: ");

		const int entered = scanf("%d", &bet_change);
		while(getchar() != '\n'){}

		if (bet_change != -1 && bet_change % 10 == 0 && bet_change <= (*cash)) {
			break;
		} else if (entered == EOF) {
			printf("Enter a number\n");
		} else if (bet_change < 0) {
			printf("Value can't be negative\n");
		} else if (bet_change != -1 && bet_change % 10 != 0) {
			printf("Value must be multiples of 10\n");
		} else if(bet_change > (*cash)) {
			printf("You don't have enough cash\n");
		}

		bet_change = -1;
	} while (1);
	(*bet) = (*bet) + bet_change;
}

void draw_to_hands(Card_t **deck, LinkedList_t *dealer_hand, LinkedList_t *player_hand) {
	while (true) {
		Card_t *card = get_random_card(deck);
		if (size(dealer_hand) < 2) {
			add(dealer_hand, card);
		} else if (size(player_hand) < 2) {
			add(player_hand, card);
		}

		if (size(dealer_hand) == 2 && size(player_hand) == 2) {
			break;
		}
	}
}

int get_last_card_position(Card_t **deck) {
	int last_card_position = DECK_SIZE - 1;
	for (; last_card_position >= 0 && deck[last_card_position] == NULL;
			last_card_position -= 1) {}
	return last_card_position;
}

void return_cards_to_deck(Card_t **deck, LinkedList_t *dealer_hand, LinkedList_t *player_hand) {
	while (size(dealer_hand) > 0) {
		Card_t *card = (Card_t*) remove_last(dealer_hand);
		int idx = card -> suit * 13 + card -> rank;
		deck[idx] = card;
	}
	while (size(player_hand) > 0) {
		Card_t *card = (Card_t*) remove_last(player_hand);
		int idx = card -> suit * 13 + card -> rank;
		deck[idx] = card;
	}
}

bool players_hit_or_stand(Card_t **deck, LinkedList_t *player_hand, int *player_points) {
	while(true) {
		char hit_or_stand;
		while(true) {
			printf("Please enter H to hit or S to stand: ");
			scanf("%c", &hit_or_stand);
			while(getchar() != '\n'){}

			if (hit_or_stand == 'H' || hit_or_stand == 'h'
					|| hit_or_stand == 'S' || hit_or_stand == 's') {
				break;
			} else {
				printf("Invalid input. ");
			}
		}

		if (hit_or_stand == 'H' || hit_or_stand == 'h') {
			add(player_hand, get_random_card(deck));
			(*player_points) = calculate_points(player_hand, false);

			show_cards("player", player_hand);

			if ((*player_points) > 21) {
				break;
			}
		} else if (hit_or_stand == 'S' || hit_or_stand == 's') {
			break;
		}
	}
	return true;
}

void dealers_hits(Card_t **deck, LinkedList_t *dealer_hand, int *dealer_points, int player_points) {
	while(true) {
		add(dealer_hand, get_random_card(deck));
		(*dealer_points) = calculate_points(dealer_hand, true);

		if (player_points < (*dealer_points) || (*dealer_points) >= 17) {
			break;
		}
	}
}


void play_black_jack() {
	srand(time(NULL));

	Card_t *deck[DECK_SIZE] = {NULL};
	LinkedList_t *dealer_hand = create_list();
	LinkedList_t *player_hand = create_list();

	unsigned int cash = INITIAL_CASH;
	unsigned int bet = 0;
	char to_continue;
	do {
		if (cash == INITIAL_CASH) {
			fill_deck(deck);
		} else {
			return_cards_to_deck(deck, dealer_hand, player_hand);
		}

		show_status(&cash, &bet);
		if (bet != 0) {
			cash += bet;
		}
		update_bet(&bet, &cash);
		cash -= bet;
		show_status(&cash, &bet);

		draw_to_hands(deck, dealer_hand, player_hand);

		show_dealer_cards(dealer_hand);
		show_cards("player", player_hand);

		int player_points = calculate_points(player_hand, false);
		if (player_points == 21) {
			printf("BlackJack!\n");
			cash += (int) (1.5 * bet);
			bet = 0;
		} else if (players_hit_or_stand(deck, player_hand, &player_points) && player_points > 21) {
			printf("Bust!\n");
			bet = 0;
		} else {
			int dealer_points = calculate_points(dealer_hand, true);
			if (dealer_points == 22) {
				printf("Dealer wins with 2 aces!\n");
			}
			else if (player_points < dealer_points) {
				printf("You lost!\n");
				bet = 0;
			} else {
				dealers_hits(deck, dealer_hand, &dealer_points, player_points);

				if (dealer_points > 21) {
					printf("Dealer's bust!\n");
					cash += 2 * bet;
					bet = 0;
					break;
				} else if (dealer_points == 21 || (dealer_points < 21 && dealer_points == player_points)) {
					printf("Tie\n");
					break;
				} else if (dealer_points < 21 && dealer_points > player_points) {
					printf("Dealer wins!\n");
					bet = 0;
					break;
				}
			}
		}

		if (DEV) {
			show_cards("dealer", dealer_hand);
		}

		if (cash < 10) {
			printf("You are out of cash to bet. Game over!\n");
		}
		
		while(true) {
			printf("Play another round? [Y/N]: ");
			scanf("%c", &to_continue);
			while(getchar() != '\n'){}

			if (to_continue == 'Y' || to_continue == 'y'
					|| to_continue == 'N' || to_continue == 'n') {
				break;
			} else {
				printf("Invalid input. ");
			}

		}

	} while(to_continue == 'Y' || to_continue == 'y');

	clear(dealer_hand);
	free(dealer_hand);
	dealer_hand = NULL;

	clear(player_hand);
	free(player_hand);
	player_hand = NULL;

//	for (int i = 0; i < DECK_SIZE; i++) {
//		if (deck[i] != NULL) {
//			free(deck[i]);
//			deck[i] = NULL;
//		}
//	}
	printf("BYE!");
}
