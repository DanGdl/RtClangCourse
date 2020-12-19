/*
 * main.c
 *
 *  Created on: Nov 27, 2020
 *      Author: max
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <limits.h>

#include "errors.h"
#include "data.h"
#include "network.h"
#include "renderer.h"


void clean(int socket_descriptor, data_address_data_t *network_data) {
	if (socket_descriptor != 0) {
		network_close_socket(&socket_descriptor);
	}
	free(network_data);
}

int main(int argc, char **argv) {
	srand(time(NULL));
	const unsigned short device_id = ((unsigned short) rand()) % USHRT_MAX;

	while(true) {
		show_message("Please enter 'S' or 's' to send data. Enter 'Q' or 'q' to exit. ");

		char c;
		const int entered = scanf("%c", &c);
		c = toupper(c);
		while(getchar() != '\n'){}

		if (entered == 1) {
			if (c == 'Q') {
				break;
			} else if (c == 'S') {
				data_address_data_t *network_data = create_address_data();
				network_data -> device_id = device_id;
				network_data -> time = time(NULL);
				FILE *pp = popen("ps -e | wc -l", "r");
				if (pp != NULL) {
					while (true) {
						char *line;
						char buf[10];
						line = fgets(buf, sizeof buf, pp);
						if (line == NULL) {
							break;
						}
						network_data -> processes = atoi(line);
					}
					pclose(pp);
				}

				const int result2 = get_data_for_interface("lo", network_data); // eth0
				if (result2 == -1) {
					log_error("Can't get address data");
					clean(0, network_data);
					continue;
				}

				uint size = sizeof(data_address_data_t);
				uint8_t buf[size];
				memset(&buf, 0, size);
				memcpy(&buf, network_data, size);
				network_data -> crc = calculate_crc(buf, size);
				memcpy(&buf, network_data, size);

				const int socket_descriptor = network_open_client_socket_by_ip("127.0.0.1", 1030);
				if (socket_descriptor == -1) {
					log_error("Can't connect");

					clean(socket_descriptor, network_data);
					continue;
				}

				const int result = network_send(&socket_descriptor, buf, 34);
				if (result == -1) {
					log_error("Can't send");
				}

				clean(socket_descriptor, network_data);
			}
		} else {
			printf("Invalid command\n");
			continue;
		}
	}
	exit(0);
}
