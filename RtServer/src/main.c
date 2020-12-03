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

int listener_d = -1;

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

void handle_shutdown(int sig) {
	if (listener_d) {
		close(listener_d);
	}
	exit(0);
}

int catch_signal(int sig, void (*handler)(int)) {
	struct sigaction action;
	action.sa_handler = handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	return sigaction(sig, &action, NULL);
}

int main(int argc, char **argv) {
	if (catch_signal(SIGINT, handle_shutdown) == -1) {
		log_error("Fail to set interrupt handler");
	}

	create_table();

	const int listener_d = network_open_server_socket(1030);
	network_listen_server_socket(listener_d);

	AddressData *datas = getAll();
	free(datas);

	uint8_t buf[34] = {0};
	while (1) {
		struct sockaddr_storage client_addr;
		unsigned int address_size = sizeof(client_addr);
		int connect_d = accept(listener_d, (struct sockaddr*) &client_addr, &address_size);
		if (connect_d == -1) {
			log_error("Accept connection failed");
		}

		// network_receive(connect_d, buf, sizeof(buf));

		read(connect_d, buf, sizeof(buf));
		AddressData *data = create_address_data();
		read_from_bytes(buf, data);

		if (data -> crc == calculate_crc(buf, 30)) {
			printf("CRC OK!!");
			save(data);

			const char msg[3] = "200";
			if (send(connect_d, &msg, strlen(msg), 0) == -1) {
				log_error("Sending response failed");
			}
		}


		close(connect_d);

//		if (!fork()) {
//			close(listener_d);
//			const char *msg = "LOL";
//			if (send(connect_d, msg, strlen(msg), 0) == -1) {
//				log_error("Sending failed");
//			}
//
//			read(connect_d, buf, sizeof(buf));
//			printf("Received: %s", buf);
//			close(connect_d);
//			exit(0);
//		}
//		close(connect_d);
	}
	network_close_socket(&listener_d);

	return 0;
}

