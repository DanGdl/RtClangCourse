/*
 * data.c
 *
 *  Created on: Nov 29, 2020
 *      Author: max
 */

#include "data.h"
#include <stdlib.h>
#include <string.h>

data_address_data_t* create_address_data() {
	data_address_data_t *data = (data_address_data_t*) malloc(sizeof(data_address_data_t));
	data -> id = -1;
	data -> device_id = 0;
	data -> processes = 0;
	data -> time = 0;
	data -> ip = 0;
	data -> mask = 0;
	data -> crc = 0;

	memset(data -> mac, 0, sizeof(data -> mac));
	return data;
}

uint get_crc_size(data_address_data_t* data) {
	return sizeof(data -> crc);
}
