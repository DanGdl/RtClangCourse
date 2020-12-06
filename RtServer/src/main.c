/*
 * main.c
 *
 *  Created on: Nov 27, 2020
 *      Author: max
 */

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "errors.h"
#include "network.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "data_dao.h"
#include <stdbool.h>


#define CONNECTION_FAILED -1

int listener_d;

/**
 * read data from socket connection
 */
int read_in(const int socket, char* buf, const int len) {
	char *s = buf;
	int slen = len;
	int c = recv(socket, s, slen, 0);
	while (c > 0 && s[c - 1] != '\n') {
		s += c;
		slen -= c;
		c = recv(socket, s, slen, 0);
	}
	if (c < 0) {
		return c;
	} else if (c == 0) {
		buf[0] = '\0';
	} else {
		s[c - 1] = '\0';
	}
	return len - slen;
}

/**
 * close server's socket
 */
void handle_shutdown(int sig) {
	if (listener_d) {
		close(listener_d);
	}
	exit(0);
}

/**
 * setup listener to receive process interruption
 */
int catch_signal(int sig, void (*handler)(int)) {
	struct sigaction action;
	action.sa_handler = handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	return sigaction(sig, &action, NULL);
}

int main(int argc, char **argv) {
	// setup listener to receive process interruption
	if (catch_signal(SIGINT, handle_shutdown) == -1) {
		log_error("Fail to set interrupt handler");
	}

	// create table in DB
	create_table();

	listener_d = network_open_server_socket(1030);
	network_listen_server_socket(listener_d);

	// AddressData *datas = getAll();
	// free(datas);

	uint8_t buf[34] = {0};
	while (true) {
		// receive connection
		struct sockaddr_storage client_addr;
		unsigned int address_size = sizeof(client_addr);
		int connect_d = accept(listener_d, (struct sockaddr*) &client_addr, &address_size);
		if (connect_d == CONNECTION_FAILED) {
			log_error("Accept connection failed");
		}

		read(connect_d, buf, sizeof(buf));
		AddressData *data = create_address_data();
		read_from_bytes(buf, data);

		if (data -> crc == calculate_crc(buf, 30)) {
			save(data);

//			const char msg[3] = "200";
//			if (send(connect_d, &msg, strlen(msg), 0) == -1) {
//				log_error("Sending response failed");
//			}
		} else {
			printf("CRC NOT OK!!");
		}

		// close connection
		close(connect_d);
	}
	// close socket
	network_close_socket(&listener_d);

	return 0;
}

