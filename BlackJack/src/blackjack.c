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

int calculate_points(const LinkedList *list) {
	int points = 0;
	int has_ace = 0;
	Node *node = list -> last;
	while (node != NULL) {
		const Card *card = (Card*) node -> data;
		points += get_card_value(card);
		if (is_ace(card)) {
			has_ace = 1;
		}
		node = node -> previous;
	}
	if (has_ace) {
		points += 10;
		if (points > 21) {
			points -= 10;
		}
	}
	return points;
}

Card* get_random_card(Card **deck) {
	do {
		const int position = rand() % DECK_SIZE;
		if (deck[position] == NULL) {
			continue;
		}
		Card *card = deck[position];
		deck[position] = NULL;
		if (card == NULL) {
			log_error("Random card is NULL!!");
		}
		return card;
	} while(1);
}

void fill_deck(Card **deck) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 13; j++) {
			const int idx = i * 13 + j;
			if (deck[idx] == NULL) {
				deck[idx] = newCard(j, i);
			}
		}
	}
}

void update_bet(const char* message, int *bet, const int *cash) {
	int bet_change = -1;
	do {
		show_message(message);

		const int entered = scanf("%d", &bet_change);
		while(getchar() != '\n'){}

		if (bet_change != -1 && bet_change % 10 == 0 && bet_change <= (*cash)) {
			break;
		} else if (entered == EOF) {
			show_warning("Enter a number\n");
		} else if (bet_change != -1 && bet_change % 10 != 0) {
			show_warning("Value must be multiples of 10\n");
		} else if(bet_change > (*cash)) {
			show_warning("You don't have enough cash\n");
		}

		bet_change = -1;
	} while (1);
	(*bet) = (*bet) + bet_change;
}

void draw_to_hands(Card **deck, LinkedList *dealer_hand, LinkedList *player_hand) {
	do {
		Card *card = get_random_card(deck);
		if (size(dealer_hand) < 2) {
			add(dealer_hand, card);
		} else if (size(player_hand) < 2) {
			add(player_hand, card);
		}

		if (size(dealer_hand) == 2 && size(player_hand) == 2) {
			break;
		}
	} while (1);
}

int get_last_card_position(Card **deck) {
	int last_card_position = DECK_SIZE - 1;
	for (; last_card_position >= 0 && deck[last_card_position] == NULL;
			last_card_position -= 1) {}
	return last_card_position;
}

void return_cards_to_deck(Card **deck, LinkedList *dealer_hand, LinkedList *player_hand) {
	int last_card_position = get_last_card_position(deck);
	for (int i = 0; i < DECK_SIZE && last_card_position > i; i++) {
		if (deck[i] == NULL) {
			deck[i] = deck[last_card_position];
			deck[last_card_position] = NULL;

			last_card_position = get_last_card_position(deck);
		}
	}
	int first_empty_position = get_last_card_position(deck) + 1;
	while (size(dealer_hand) > 0) {
		Card *card = (Card*) remove_last(dealer_hand);
		deck[first_empty_position] = card;
		first_empty_position += 1;
	}
	while (size(player_hand) > 0) {
		Card *card = (Card*) remove_last(player_hand);
		deck[first_empty_position] = card;
		first_empty_position += 1;
	}
}

void play_black_jack() {
	srand(time(NULL));

	Card *deck[DECK_SIZE] = {NULL};
	LinkedList *dealer_hand = createList();
	LinkedList *player_hand = createList();

	int cash = INITIAL_CASH;
	int bet = 0;
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
		update_bet(request_bet_update(&bet), &bet, &cash);
		cash -= bet;
		show_status(&cash, &bet);

		draw_to_hands(deck, dealer_hand, player_hand);

		show_dealer_cards(dealer_hand);
		show_player_cards(player_hand);

		int player_points = calculate_points(player_hand);
		if (player_points == 21) {
			show_message("BlackJack!\n");
			cash += (int) (1.5 * bet);
			bet = 0;
		} else {
			// hit or stand
			int is_burst = 0;
			do {
				char bet_or_stand;
				do {
					show_message("Please enter H to hit or S to stand: ");
					scanf("%c", &bet_or_stand);
					while(getchar() != '\n'){}

					if (bet_or_stand == 'H' || bet_or_stand == 'h'
							|| bet_or_stand == 'S' || bet_or_stand == 's') {
						break;
					} else {
						show_warning("Invalid input. ");
					}
				} while(1);

				if (bet_or_stand == 'H' || bet_or_stand == 'h') {
					add(player_hand, get_random_card(deck));
					player_points = calculate_points(player_hand);

					show_cards("player", player_hand);

					if (player_points > 21) {
						show_message("Bust!\n");
						bet = 0;
						is_burst = 1;
						break;
					}
				} else if (bet_or_stand == 'S' || bet_or_stand == 's') {
					break;
				}
			} while(1);

			// dealer
			if (is_burst == 0) {
				int dealer_points = calculate_points(dealer_hand);
				if (player_points < dealer_points) {
					show_message("You lost!\n");
					bet = 0;
				} else {
					do {
						add(dealer_hand, get_random_card(deck));
						dealer_points = calculate_points(dealer_hand);

						if (player_points < dealer_points || dealer_points >= 17) {
							if (dealer_points > 21) {
								show_message("Dealer's bust!\n");
								cash += 2 * bet;
								bet = 0;
								break;
							} else if (dealer_points == 21 || (dealer_points < 21 && dealer_points == player_points)) {
								show_message("Tie\n");
								break;
							} else if (dealer_points < 21 && dealer_points > player_points) {
								show_message("Dealer wins!\n");
								bet = 0;
								break;
							}
						}
					} while(1);
				}
			}
		}

		if (DEV) {
			show_cards("dealer", dealer_hand);
		}

		if (cash < 10) {
			show_message("You are out of cash to bet. Game over!\n");
		}
		
		do {
			show_message("Play another round? [Y/N]: ");
			scanf("%c", &to_continue);
			while(getchar() != '\n'){}

			if (to_continue == 'Y' || to_continue == 'y'
					|| to_continue == 'N' || to_continue == 'n') {
				break;
			} else {
				show_warning("Invalid input. ");
			}

		} while(1);

	} while(to_continue == 'Y' || to_continue == 'y');

	clear(dealer_hand);
	free(dealer_hand);
	dealer_hand = NULL;

	clear(player_hand);
	free(player_hand);
	player_hand = NULL;

	for (int i = 0; i < DECK_SIZE; i++) {
		if (deck[i] != NULL) {
			free(deck[i]);
			deck[i] = NULL;
		}
	}
	printf("BYE!");
}
