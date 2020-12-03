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
	data -> id = -1;
	data -> device_id = 0;
	data -> time = 0;
	data -> processes = 0;
	data -> ip = 0;
	data -> mask = 0;
	data -> mac = (unsigned char*) malloc(7 * sizeof(char));
	data -> mac[6] = '\0';
	data -> crc = 0;
	return data;
}

void write_to_bytes(uint8_t buffer[], AddressData *data) {
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

	// processes
	buffer[12] = data -> processes >> 24;
	buffer[13] = data -> processes >> 16;
	buffer[14] = data -> processes >> 8;
	buffer[15] = data -> processes;

	// ip
	buffer[16] = data -> ip >> 24;
	buffer[17] = data -> ip >> 16;
	buffer[18] = data -> ip >> 8;
	buffer[19] = data -> ip;

	// mask
	buffer[20] = data -> mask >> 24;
	buffer[21] = data -> mask >> 16;
	buffer[22] = data -> mask >> 8;
	buffer[23] = data -> mask;

	// mac
	buffer[24] = data -> mac[0];
	buffer[25] = data -> mac[1];
	buffer[26] = data -> mac[2];
	buffer[27] = data -> mac[3];
	buffer[28] = data -> mac[4];
	buffer[29] = data -> mac[5];

	// crc
	buffer[30] = data -> crc >> 24;
	buffer[31] = data -> crc >> 16;
	buffer[32] = data -> crc >> 8;
	buffer[33] = data -> crc;
}

void read_from_bytes(uint8_t buffer[], AddressData *data) {

	// device_id
	data -> device_id = (0 | buffer[0] << 8) | buffer[1];

	// time
	data -> time = // ((long int) 0 | buffer[2] << 72) | (buffer[3] << 64) |
			((long int) 0 | buffer[4] << 56) | (buffer[5] << 48)
			| (buffer[6] << 40) | (buffer[7] << 32) | (buffer[8] << 24) | (buffer[9] << 16)
			| (buffer[10] << 8) | buffer[11];

	// write time
	data -> processes = (0 | buffer[12] << 24) | (buffer[13] << 16) | (buffer[14] << 8) | buffer[15];

	// ip
	data -> ip = (0 | buffer[16] << 24) | (buffer[17] << 16) | (buffer[18] << 8) | buffer[19];

	// mask
	data -> mask = (0 | buffer[20] << 24) | (buffer[21] << 16) | (buffer[22] << 8) | buffer[23];

	// mac
	data -> mac[0] = buffer[24];
	data -> mac[1] = buffer[25];
	data -> mac[2] = buffer[26];
	data -> mac[3] = buffer[27];
	data -> mac[4] = buffer[28];
	data -> mac[5] = buffer[29];

	// crc
	data -> crc = (0 | buffer[30] << 24) | (buffer[31] << 16) | (buffer[32] << 8) | buffer[33];
}
