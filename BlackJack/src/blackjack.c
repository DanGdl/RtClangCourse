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

#define FILL_DECK(deck) \
		for (int i = 0; i < 4; i++) {\
			int idx =i * 13;\
			for (int j = 0; j < 13; j++) {\
				deck[idx + j] = new_card(j, i);\
			}}


Card_t deck[DECK_SIZE];
bool has_card_n_deck[DECK_SIZE] = {true};

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
		// if (is_dealer && aces == 2 && points + 20 <= 22) {
		// 	points += 22;
		// } else
		if (points + 10 <= 21) {
			points += 10;
		}
	}
	return points;
}

Card_t* get_random_card() {
	while(true) {
		const int position = rand() % DECK_SIZE;
		if (has_card_n_deck[position]) {
			Card_t *card = &deck[position];
			has_card_n_deck[position] = false;
			return card;
		}
	}
}

void update_bet(unsigned int *bet, const unsigned int *cash) {
	int bet_change = -1;
	while (true) {
		printf("If you want to add bet, please enter amount (multiples of 10)%s: ", (*bet) == 0 ? "" : " or 0");

		const int entered = scanf("%d", &bet_change);
		while(getchar() != '\n'){}


		if (((*bet) == 0 && bet_change == 0)) {
			printf("Value can't be zero\n");
		} else if (bet_change != -1 && bet_change % 10 == 0 && bet_change <= (*cash)) {
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
	}
	(*bet) = (*bet) + bet_change;
}

void draw_to_hands(LinkedList_t *dealer_hand, LinkedList_t *player_hand) {
	while (true) {
		Card_t *card = get_random_card();
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

void return_cards_to_deck(LinkedList_t *dealer_hand, LinkedList_t *player_hand) {
	while (size(dealer_hand) > 0) {
		Card_t *card = (Card_t*) remove_last(dealer_hand);
		has_card_n_deck[card -> suit * 13 + card -> rank] = true;
	}
	while (size(player_hand) > 0) {
		Card_t *card = (Card_t*) remove_last(player_hand);
		has_card_n_deck[card -> suit * 13 + card -> rank] = true;
	}
}

bool players_hit_or_stand(LinkedList_t *player_hand, int *player_points) {
	while(true) {
		char hit_or_stand;
		while(true) {
			printf("Please enter H to hit or S to stand: ");
			scanf("%c", &hit_or_stand);
			hit_or_stand = toupper(hit_or_stand);
			while(getchar() != '\n'){}

			if (hit_or_stand == 'H'|| hit_or_stand == 'S') {
				break;
			} else {
				printf("Invalid input. ");
			}
		}

		if (hit_or_stand == 'H') {
			add(player_hand, get_random_card());
			(*player_points) = calculate_points(player_hand, false);

			show_cards("player", player_hand);

			if ((*player_points) > 21) {
				break;
			}
		} else if (hit_or_stand == 'S') {
			break;
		}
	}
	return true;
}


void dealers_hits(LinkedList_t *dealer_hand, int *dealer_points, int player_points) {
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

	FILL_DECK(deck);
	memset(has_card_n_deck, true, DECK_SIZE * sizeof(bool));
	LinkedList_t *dealer_hand = create_list();
	LinkedList_t *player_hand = create_list();

	unsigned int cash = INITIAL_CASH;
	unsigned int bet = 0;
	char to_continue;
	do {
		return_cards_to_deck(dealer_hand, player_hand);

		show_status(&cash, &bet);
		if (bet != 0) {
			cash += bet;
		}
		update_bet(&bet, &cash);
		cash -= bet;
		show_status(&cash, &bet);

		draw_to_hands(dealer_hand, player_hand);

		show_dealer_cards(dealer_hand);
		show_cards("player", player_hand);

		int player_points = calculate_points(player_hand, false);
		if (player_points == 21) {
			printf("BlackJack!\n");
			cash += (int) (1.5 * bet);
			bet = 0;
		} else if (players_hit_or_stand(player_hand, &player_points) && player_points > 21) {
			printf("Bust!\n");
			bet = 0;
		} else {
			int dealer_points = calculate_points(dealer_hand, true);
			// if (dealer_points == 22) {
			// 	printf("Dealer wins with 2 aces!\n");
			// }
			// else
			if (player_points < dealer_points) {
				printf("You lost!\n");
				bet = 0;
			} else {
				dealers_hits(dealer_hand, &dealer_points, player_points);

				if (dealer_points > 21) {
					printf("Dealer's bust!\n");
					cash += 2 * bet;
					bet = 0;
				} else if (dealer_points == 21) {
					printf("Dealer's BlackJack!\n");
					bet = 0;
				} else if ((dealer_points < 21 && dealer_points == player_points)) {
					printf("Tie\n");
				} else if (dealer_points < 21 && dealer_points > player_points) {
					printf("Dealer wins!\n");
					bet = 0;
				}
			}
		}

		if (DEV) {
			show_cards("dealer", dealer_hand);
		}

		if (cash < 10) {
			printf("You are out of cash to bet. Game over!\n");
			break;
		}
		
		while(true) {
			printf("Play another round? [Y/N]: ");
			scanf("%c", &to_continue);
			to_continue = toupper(to_continue);
			while(getchar() != '\n'){}

			if (to_continue == 'Y' || to_continue == 'N') {
				break;
			} else {
				printf("Invalid input. ");
			}

		}

	} while(to_continue == 'Y');

	clear(dealer_hand);
	free(dealer_hand);
	dealer_hand = NULL;

	clear(player_hand);
	free(player_hand);
	player_hand = NULL;
	printf("BYE!");
}
