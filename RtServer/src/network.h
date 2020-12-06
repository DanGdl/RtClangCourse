/*
 * network.h
 *
 *  Created on: Nov 28, 2020
 *      Author: max
 */
#ifndef SRC_NETWORK_H_
#define SRC_NETWORK_H_

#include "data.h"

typedef uint32_t crc;

#define WIDTH  (8 * sizeof(crc))
#define TOPBIT (1 << (WIDTH - 1))
#define POLYNOMIAL 0xD8  /* 11011 followed by 0's */


void network_close_socket(const int *socket_descriptor);

crc calculate_crc(uint8_t const message[], const int nBytes);

int network_open_server_socket(int port);

void network_listen_server_socket(int socket_descriptor);

#endif
