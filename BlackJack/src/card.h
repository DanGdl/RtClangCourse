#ifndef BLACKJACK_CARD
#define BLACKJACK_CARD

enum Suits {
	SPADES, DIAMONDS, HEARTS, CLUBS
};

typedef struct Card {
	int rank;
	int suit;
} Card;

void generateCard(Card *card, int ranks, int suits);

Card newRandomCard(int ranks, int suits);

Card* newCard(int rank, int suit);

int compareCard(Card *card1, Card *card2);

char* get_card_suit(const Card *card);

char* get_card_rank_name(const Card *card);

int get_card_value(const Card *card);

Card copyCard(Card *card);

void print_card(Card *card);

int is_ace(const Card *card);

#endif
