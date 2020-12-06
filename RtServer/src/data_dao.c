/*
 * data_dao.c
 *
 *  Created on: Dec 1, 2020
 *      Author: max
 */

#include "data_dao.h"
#include "errors.h"

#include <libpq-fe.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * creates table to save data in DB
 */
void create_table() {
	PGconn *conn = PQconnectdb("user=postgres password=123456 dbname=postgres");

	if (PQstatus(conn) == CONNECTION_BAD) {
		fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(conn));
		PQfinish(conn);
		exit(1);
	}

	PGresult *res = PQexec(conn, "CREATE TABLE IF NOT EXISTS device_datas (" \
			"id integer PRIMARY KEY GENERATED BY DEFAULT AS IDENTITY," \
			"device_id integer," \
			"time integer," \
			"ip integer," \
			"mask integer," \
			"mac character(7)" \
			")");

	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		fprintf(stderr, "Create table query failed: %s\n", PQerrorMessage(conn));
		PQfinish(conn);
		exit(1);
	}
	PQclear(res);
	PQfinish(conn);
}

/**
 * saves data in DB
 */
void save(AddressData *data) {
	// transaction?
	PGconn *conn = PQconnectdb("user=postgres password=123456 dbname=postgres");

	if (PQstatus(conn) == CONNECTION_BAD) {
		fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(conn));
		PQfinish(conn);
		return;
	}
	char *query = (char*) malloc(200 * sizeof(char));
	memset(query, 0, 200 * sizeof(char));

	if (data -> id == -1) {
		sprintf(query, "INSERT INTO device_datas(device_id, time, ip, mask, mac) VALUES(%d, %li, %d, %u, '%s')",
				data -> device_id, data -> time, data -> ip, data -> mask, data -> mac);
	} else {
		sprintf(query, "UPDATE device_datas SET device_id=%d, time=%li, ip=%d, mask=%ui, mac=%s WHERE id=%lli",
						data -> device_id, data -> time, data -> ip, data -> mask, data -> mac, data -> id);
	}
	PGresult *res = PQexec(conn, query);
	free(query);
	if (PQresultStatus(res) != PGRES_COMMAND_OK) {
		PQclear(res);
		PQfinish(conn);
		return;
	}
	PQclear(res);
	PQfinish(conn);
}

AddressData* getAll() {
	PGconn *conn = PQconnectdb("user=postgres password=123456 dbname=postgres");

	if (PQstatus(conn) == CONNECTION_BAD) {
		fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(conn));
		return NULL;
	}

	PGresult *res = PQexec(conn, "SELECT * FROM device_datas");

	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
		printf("No data retrieved\n");
		PQclear(res);
		return NULL;
	}

	int rows = PQntuples(res);

	for(int i = 0; i < rows; i++) {
		printf("id %s, device_id %s, time %s, ip %s, mask %s, mac '%s'\n",
				PQgetvalue(res, i, 0), PQgetvalue(res, i, 1), PQgetvalue(res, i, 2),
				PQgetvalue(res, i, 3), PQgetvalue(res, i, 4), PQgetvalue(res, i, 5));
	}

	PQclear(res);
	PQfinish(conn);
	return NULL;
}
