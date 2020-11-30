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


int network_open_client_socket_by_ip(const char *ip, const int port);

int network_open_client_socket(const char *address, const char *port);

void network_close_socket(const int *socket_descriptor);

int network_send(const int *socket_descriptor, uint8_t *message, const int length);

int network_receive(const int *socket_descriptor, uint8_t *buf, int buffer_size);

int get_str_data_for_interface(const char *name, char **network_data);

int get_data_for_interface(const char *name, AddressData *network_data);

crc calculate_crc(uint8_t const message[], const int nBytes);

int network_open_server_socket(int port);

void network_listen_server_socket(int socket_descriptor);

#endif
