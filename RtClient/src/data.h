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
	unsigned int crc;
	unsigned short device_id;
	long int time;
	int processes;
	uint32_t ip;
	uint32_t mask;
	unsigned char mac[6];
};

typedef struct data_address_data data_address_data_t;

data_address_data_t* create_address_data();

uint get_crc_size(data_address_data_t* data);

#endif
