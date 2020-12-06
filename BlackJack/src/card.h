#ifndef BLACKJACK_CARD
#define BLACKJACK_CARD

enum Suits {
	SPADES, DIAMONDS, HEARTS, CLUBS
};

typedef struct Card_t {
	int rank;
	int suit;
} Card_t;


//Card_t* new_card(int rank, int suit);

Card_t new_card(int rank, int suit);

char* get_card_suit(const Card_t *card);

char* get_card_rank_name(const Card_t *card);

int get_card_value(const Card_t *card);

void print_card(const Card_t *card);

int is_ace(const Card_t *card);

#endif
