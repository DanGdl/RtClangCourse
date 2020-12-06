/*
 * errors.c
 *
 *  Created on: Nov 27, 2020
 *      Author: max
 */
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void log_error(const char *message) {
	fprintf(stderr, "%s: %s", message, strerror(errno));
}

void log_error_exit(const char *message) {
	log_error(message);
	exit(1);
}
