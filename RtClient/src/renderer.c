/*
 * renderer.c
 *
 *  Created on: Nov 24, 2020
 *      Author: max
 */
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

void show_message(const char *message) {
	printf(message);
}

