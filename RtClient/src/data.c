/*
 * data.c
 *
 *  Created on: Nov 29, 2020
 *      Author: max
 */

#include "data.h"
#include <stdlib.h>

AddressData* create_address_data() {
	AddressData *data = (AddressData*) malloc(sizeof(AddressData));
	data -> device_id = 0;
	data -> time = 0;
	data -> processes = 0;
	data -> ip = 0;
	data -> mask = 0;
	data -> mac = (unsigned char*) malloc(6 * sizeof(char));
	data -> crc = 0;
	return data;
}


void to_binary(unsigned k) {
	while (k) {
	    if (k & 1)
	        printf("1");
	    else
	        printf("0");

	    k >>= 1;
	}
	printf("\n");
}


void write_to_bytes(uint8_t buffer[], int length, AddressData *data) {
	// write device_id
	buffer[0] = data -> device_id >> 8;
	buffer[1] = data -> device_id;

	// write time
	buffer[2] = 0; // data -> time >> 72;
	buffer[3] = 0; // data -> time >> 64;
	buffer[4] = data -> time >> 56;
	buffer[5] = data -> time >> 48;
	buffer[6] = data -> time >> 40;
	buffer[7] = data -> time >> 32;
	buffer[8] = data -> time >> 24;
	buffer[9] = data -> time >> 16;
	buffer[10] = data -> time >> 8;
	buffer[11] = data -> time;

	buffer[12] = data -> processes >> 24;
	buffer[13] = data -> processes >> 16;
	buffer[14] = data -> processes >> 8;
	buffer[15] = data -> processes;

	buffer[16] = data -> ip >> 24;
	buffer[17] = data -> ip >> 16;
	buffer[18] = data -> ip >> 8;
	buffer[19] = data -> ip;

	buffer[20] = data -> mask >> 24;
	buffer[21] = data -> mask >> 16;
	buffer[22] = data -> mask >> 8;
	buffer[23] = data -> mask;

	buffer[24] = data -> mac[0];
	buffer[25] = data -> mac[1];
	buffer[26] = data -> mac[2];
	buffer[27] = data -> mac[3];
	buffer[28] = data -> mac[4];
	buffer[29] = data -> mac[5];
}
