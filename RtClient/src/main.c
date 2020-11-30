/*
 * main.c
 *
 *  Created on: Nov 27, 2020
 *      Author: max
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#include "errors.h"
#include "data.h"
#include "network.h"
#include "renderer.h"


void clean(int socket_descriptor, AddressData *network_data) {
	if (socket_descriptor != 0) {
		network_close_socket(&socket_descriptor);
	}
	free(network_data);
}

int main(int argc, char **argv) {
//	char **network_data_str = (char**) malloc(3 * sizeof(char));
//	network_data_str[0] = (char*) malloc(15 * sizeof(char));
//	network_data_str[1] = (char*) malloc(15 * sizeof(char));
//	network_data_str[2] = (char*) malloc(17 * sizeof(char));
//	const int result1 = get_str_data_for_interface("lo", network_data_str); // eth0
//	if (result1 == -1) {
//		log_error("Can't get address data");
//	}
//	printf("Interface lo\t, Address: %s\t, Mask %s\tMAC: %s\n", network_data_str[0], network_data_str[1], network_data_str[2]);

	srand(time(NULL));
	const unsigned short device_id = ((unsigned short) rand()) % USHRT_MAX;

	do {
		show_message("Please enter 'S' or 's' to send data. Enter 'Q' or 'q' to exit. ");

		char c;
		const int entered = scanf("%c", &c);
		while(getchar() != '\n'){}

		if ((c == 'Q' || c == 'q') && entered == 1) {
			break;
		} else if ((c == 'S' || c == 's') && entered == 1) {
			AddressData *network_data = create_address_data();
			network_data -> device_id = device_id;
			network_data -> time = time(NULL);
			FILE *pp = popen("ps -e | wc -l", "r");
			if (pp != NULL) {
				while (1) {
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
			// printf("Interface lo\t, Address: %u\t, Mask %u\tMAC: %s\n", network_data -> ip, network_data -> mask, network_data -> mac);

			uint8_t buf[34];
			write_to_bytes(buf, 34, network_data);

			// TODO: to array
			network_data -> crc = calculate_crc(buf, 30);

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
//			uint8_t rec[256];
//			network_receive(socket_descriptor, rec, 255);

			clean(socket_descriptor, network_data);
		}
	} while (1);

	exit(0);
}



// #include <sys/wait.h>
//			int fd[2];
//			if (pipe(fd) == -1) {
//				// error
//			}
//			pid_t pid = fork();
//			if (pid == -1) {
//			 // error
//			}
//			if (!pid) {
//				dup2(fd[1], 1);
//				close(fd[0]);
//				if (execlp("/bin/sh", "ps -e | wc -l", NULL) == -1) {
//					// error
//				}
//			}
//			dup2(fd[0], 0);
//			close(fd[1]);
//			char line[255];
//			while (fgets(line, 255, stdin)) {
//				printf("%s", line);
//			}
//			int pid_status;
//			if (waitpid(pid, &pid_status, 0) == -1) {
//				// error
//			}


