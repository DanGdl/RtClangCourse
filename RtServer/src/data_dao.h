/*
 * data_dao.h
 *
 *  Created on: Dec 1, 2020
 *      Author: max
 */

#ifndef SRC_DATA_DAO_H_
#define SRC_DATA_DAO_H_

#include "data.h"

void create_table();

void save(AddressData *data);

AddressData* getAll();

#endif /* SRC_DATA_DAO_H_ */
