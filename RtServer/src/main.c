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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

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

	// create socket
	int listener_d = socket(PF_INET, SOCK_STREAM, 0);
	if (listener_d == -1) {
		log_error("Create socket failed");
	}

	// bind params
	struct sockaddr_in name;
	name.sin_family = PF_INET;
	name.sin_port = (in_port_t) htons(30000);
	name.sin_addr.s_addr = htonl(INADDR_ANY);

	// set reuse option
	int reuse = 1;
	if (setsockopt(listener_d, SOL_SOCKET, SO_REUSEADDR, (char*) &reuse, sizeof(int)) == 1) {
		log_error("Set reuse failed");
	}

	// bind socket
	const int c = bind(listener_d, (struct sockaddr*) &name, sizeof(name));
	if (c == -1) {
		log_error("Bind failed");
	}
	if (listen(listener_d, 10) == -1) {
		log_error("Listen failed");
	}

	char buf[255] = {0};
	while (1) {
		struct sockaddr_storage client_addr;
		unsigned int address_size = sizeof(client_addr);
		int connect_d = accept(listener_d, (struct sockaddr*) &client_addr, &address_size);
		if (connect_d == -1) {
			log_error("Accept connection failed");
		}

		if (!fork()) {
			close(listener_d);
			const char *msg = "Server with protocol Tuk-tuk\r\nVersion 1.0\r\nTu-tuk!\r\n>";
			if (send(connect_d, msg, strlen(msg), 0) == -1) {
				log_error("Sending failed");
			}

			read(connect_d, buf, sizeof(buf));
			printf("Received: %s", buf);
			close(connect_d);
			exit(0);
		}
		close(connect_d);
	}
	close(listener_d);

	return 0;
}

