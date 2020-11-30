/*
 * data.h
 *
 *  Created on: Nov 29, 2020
 *      Author: max
 */
#ifndef SRC_DATA_H_
#define SRC_DATA_H_

#include <unictype.h>

typedef struct AddressData {
	unsigned short device_id; // 2 Bytes
	long int time; // 10 bytes
	int processes; // 4 Bytes
	uint32_t ip; // 8 Bytes
	uint32_t mask;
	unsigned char *mac; // 6 Bytes
	unsigned int crc; // 4 Bytes
	// total 34 bytes
} AddressData;

AddressData* create_address_data();

void write_to_bytes(uint8_t buffer[], AddressData *data);

void read_from_bytes(uint8_t buffer[], AddressData *data);

#endif
