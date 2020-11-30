/*
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

int network_open_client_socket_by_ip(const char *ip, const int port) {
	int socket_descriptor = socket(PF_INET, SOCK_STREAM, 0);
	if (socket_descriptor == -1) {
		log_error("Create socket failed");
	}

	struct sockaddr_in si;
	memset(&si, 0, sizeof(si));
	si.sin_family = PF_INET;
	si.sin_addr.s_addr = inet_addr(ip);
	si.sin_port = htons(port);
	const int c = connect(socket_descriptor, (struct sockaddr*) &si, sizeof(si));
	if (c == -1) {
		return -1;
	}
	return socket_descriptor;
}

int network_open_client_socket(const char *address, const char *port) {
	struct addrinfo hints;
	struct addrinfo *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if (getaddrinfo(address, port, &hints, &res) == -1) {
		log_error("Can't resolve address");
	}
	const int socket_descriptor = socket(res -> ai_family, res -> ai_socktype, res -> ai_protocol);
	if (socket_descriptor == -1) {
		log_error("Create socket failed");
	}
	const int c = connect(socket_descriptor, res -> ai_addr, res -> ai_addrlen);
	freeaddrinfo(res);
	if (c == -1) {
		log_error("Can't connect");
	}
	return socket_descriptor;
}

void network_close_socket(int *socket_descriptor) {
	if (socket_descriptor != NULL && *socket_descriptor != 0) {
		close(*socket_descriptor);
	}
}

int network_send(const int *socket_descriptor, uint8_t *message, const int length) {
	return send(*socket_descriptor, message, length, 0);
}

int network_receive(const int *socket_descriptor, char *buf, int buffer_size) {
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

int get_str_data_for_interface(const char *name, char **network_data) {
	struct ifaddrs *addresses;
	const int result = getifaddrs(&addresses);
	if (result == -1) {
		return -1;
	}

	for (struct ifaddrs *ifa = addresses; ifa != NULL; ifa = ifa -> ifa_next) {
		if (ifa -> ifa_addr == NULL || strcmp(name, ifa -> ifa_name) != 0) {
			continue;
		}
		if (ifa -> ifa_addr -> sa_family == AF_INET) {

		   struct sockaddr_in *sa = (struct sockaddr_in *) ifa -> ifa_addr;
		   char *value = inet_ntoa(sa -> sin_addr);
		   strcpy(network_data[0], value);

		   sa = (struct sockaddr_in *) ifa -> ifa_netmask;
		   value = inet_ntoa(sa -> sin_addr);
		   strcpy(network_data[1], value);
		}
		if (ifa -> ifa_addr -> sa_family == AF_PACKET) {
			 struct sockaddr_ll *s = (struct sockaddr_ll*) ifa -> ifa_addr;
			 for (int i = 0; i < (s -> sll_halen); i++) {
				 if (i + 1 == s -> sll_halen) {
					 sprintf(&(network_data[2][i * 3]), "%02x", s -> sll_addr[i]);
				 } else {
					 sprintf(&(network_data[2][i * 3]), "%02x%c", s -> sll_addr[i], ':');
				 }
			 }
		}
	}
	freeifaddrs(addresses);
	return 0;
}

int get_data_for_interface(const char *name, AddressData *network_data) {
	struct ifaddrs *addresses;
	const int result = getifaddrs(&addresses);
	if (result == -1) {
		return -1;
	}

	for (struct ifaddrs *ifa = addresses; ifa != NULL; ifa = ifa -> ifa_next) {
		if (ifa -> ifa_addr == NULL || strcmp(name, ifa -> ifa_name) != 0) {
			continue;
		}
		if (ifa -> ifa_addr -> sa_family == AF_INET) {
		   struct sockaddr_in *sa = (struct sockaddr_in *) ifa -> ifa_addr;
		   memcpy(&(network_data -> ip), &(sa -> sin_addr.s_addr), sizeof(uint32_t));

		   sa = (struct sockaddr_in *) ifa -> ifa_netmask;
		   memcpy(&(network_data -> mask), &(sa -> sin_addr.s_addr), sizeof(uint32_t));
		}
		if (ifa -> ifa_addr -> sa_family == AF_PACKET) {
			 struct sockaddr_ll *s = (struct sockaddr_ll*) ifa -> ifa_addr;
			 for (int i = 0; i < (s -> sll_halen); i++) {
				memcpy(&(network_data -> mac[i]), &(s -> sll_addr[i]), sizeof(unsigned char));
			 }
		}
	}
	freeifaddrs(addresses);
	return 0;
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
