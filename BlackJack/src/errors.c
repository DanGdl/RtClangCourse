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

void log_error_exit(const char *message) {
	fprintf(stderr, "%s: %d", message, strerror(errno));
	exit(1);
}
