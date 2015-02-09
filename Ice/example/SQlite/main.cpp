/*
 * main.cpp
 *
 *  Created on: 2013-4-11
 *      Author: Ice
 */

#include <iostream>
#include <thread>
#include "sqlite3.h"

using namespace std;

int rs_cb(void *data, int num, char **values, char ** names) {
	static bool print_head = true;
	if (print_head) {
		for (int i=0; i< num; i++) {
			cout << names[i] << "\t" ;
		}
		cout << endl;
		print_head = false;
	}
	for (int i=0; i< num; i++) {
		cout << values[i] << "\t" ;
	}
	cout << endl;
	return 0;
}

int main(int argv, char** argc) {
	thread t;
	thread::id id = this_thread::get_id();
	cout << "0x" << hex << id << dec << endl;

	sqlite3 *db = NULL;
	char *errmsg = NULL;
	int rc;
	rc = sqlite3_open("test.db", &db);

	if (rc){
		sqlite3_close(db);
		cout << "Failed to open database 1! " << errmsg << endl;
		exit(1);
	}

	string sqlstr = "create table test(name text, age integer)";
	rc = sqlite3_exec(db, sqlstr.c_str(), 0, 0, &errmsg);
	if (rc != SQLITE_OK) {
		//cout << "Failed to open database 2! " << errmsg << endl;
		//exit(1);
	}

	sqlstr = "insert into test values ('ice', 28)";
	rc = sqlite3_exec(db, sqlstr.c_str(), 0, 0, &errmsg);
	if (rc != SQLITE_OK) {
		cout << "Failed to open database 3! " << errmsg << endl;
		exit(1);
	}

	sqlstr = "select * from test";
	rc = sqlite3_exec(db, sqlstr.c_str(), rs_cb, 0, &errmsg);
	if (rc != SQLITE_OK) {
		cout << "Failed to open database 4! " << errmsg << endl;
		exit(1);
	}

	sqlite3_close(db);
	return 0;
}

