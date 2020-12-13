/*
 * data.h
 *
 *  Created on: Nov 29, 2020
 *      Author: max
 */
#ifndef SRC_DATA_H_
#define SRC_DATA_H_

#include <unictype.h>

struct data_address_data {
	long long id;				//
	unsigned int crc;			// 4 Bytes
	unsigned short device_id;	// 2 Bytes
	long int time;				// 10 bytes
	int processes;				// 4 Bytes
	uint32_t ip;				// 8 Bytes
	uint32_t mask;				// 4 bytes
	unsigned char mac[6];		// 6 Bytes
	// total 34 bytes
};

typedef struct data_address_data data_address_data_t;

data_address_data_t* create_address_data();

uint get_crc_size(data_address_data_t* data);

#endif
