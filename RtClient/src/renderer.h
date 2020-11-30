/*
 * renderer.h
 *
 *  Created on: Nov 24, 2020
 *      Author: max
 */

#ifndef RENDERER_H_
#define RENDERER_H_

void show_status(const int *cash, const int *bet);

const char* request_bet_update(const int *bet);

void show_warning(const char *message);

void show_message(const char *message);

#endif /* SRC_RENDERER_H_ */
