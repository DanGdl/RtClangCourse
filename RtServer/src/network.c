/**
 * network.c
 *
 *  Created on: Nov 28, 2020
 *      Author: max
 */
#define _GNU_SOURCE     /* To get defns of NI_MAXSERV and NI_MAXHOST */
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <linux/if_link.h>
#include <ifaddrs.h>
#include <netpacket/packet.h>

#include "network.h"
#include "errors.h"


void network_close_socket(const int *socket_descriptor) {
	if (socket_descriptor != NULL && *socket_descriptor != 0) {
		close(*socket_descriptor);
	}
}

int network_receive(const int *socket_descriptor, uint8_t *buf, int buffer_size) {
	int total_received = 0;
	int bytes_received = 0;
	do {
		bytes_received = recv(*socket_descriptor, buf, buffer_size, 0);
		if (bytes_received == -1) {
			return -1;
		}
		total_received += bytes_received;
		buf[bytes_received] = '\0';
		printf("%s", buf);
	}
	while (bytes_received);
	return total_received;
}

crc calculate_crc(uint8_t const message[], int nBytes) {
	crc  remainder = 0;

	// Perform modulo-2 division, a byte at a time.
	for (int byte = 0; byte < nBytes; ++byte) {
		// Bring the next byte into the remainder.
		remainder ^= (message[byte] << (WIDTH - 8));

		 // Perform modulo-2 division, a bit at a time.
		for (uint8_t bit = 8; bit > 0; --bit) {
			// Try to divide the current data bit.
			if (remainder & TOPBIT) {
				remainder = (remainder << 1) ^ POLYNOMIAL;
			} else {
				remainder = (remainder << 1);
			}
		}
	}

	/*
	 * The final remainder is the CRC result.
	 */
	return remainder;
}

int network_open_server_socket(int port) {
	const int listener_d = socket(PF_INET, SOCK_STREAM, 0);
	if (listener_d == -1) {
		log_error_exit("Create socket failed");
	}
	// bind params
	struct sockaddr_in name;
	name.sin_family = PF_INET;
	name.sin_port = (in_port_t) htons(port);
	name.sin_addr.s_addr = htonl(INADDR_ANY);

	// set reuse option
	int reuse = 1;
	if (setsockopt(listener_d, SOL_SOCKET, SO_REUSEADDR, (char*) &reuse, sizeof(int)) == 1) {
		log_error_exit("Set reuse failed");
	}

	// bind socket
	const int c = bind(listener_d, (struct sockaddr*) &name, sizeof(name));
	if (c == -1) {
		log_error_exit("Bind failed");
	}
	return listener_d;
}

void network_listen_server_socket(int socket_descriptor) {
	if (listen(socket_descriptor, 10) == -1) {
		log_error_exit("Listen failed");
	}
}
