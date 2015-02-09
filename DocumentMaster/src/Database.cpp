/*
 * Datebase.cpp
 *
 *  Created on: 2014Äê3ÔÂ7ÈÕ
 *      Author: Ice
 */

#include "Database.h"

#include "ConsoleLog.h"

#include <fstream>

using namespace std;

shared_ptr<Database> Database::smDb;

Database::Database(const string &dbname):mName(dbname),mDb(NULL) {

}

Database::~Database() {
	sqlite3_close(mDb);
}

void Database::init(const string &dbname) {
	smDb = make_shared<Database>(dbname);
}

shared_ptr<Database> Database::getSelf() {
	return smDb;
}

bool Database::runSql(const std::string &sqlstr) {
	sqlite3_stmt *ppStmt = NULL;
	int rc = sqlite3_prepare(mDb, sqlstr.c_str(), -1, &ppStmt, NULL);
	if (rc != SQLITE_OK) {
		Error << sqlite3_errstr(rc) << "(" << rc << "): " << sqlstr << endl;
		return false;
	}
	rc = sqlite3_step(ppStmt);
	if (rc != SQLITE_DONE) {
		sqlite3_finalize(ppStmt);
		Error << sqlite3_errstr(rc) << "(" << rc << "): " << sqlstr << endl;
		return false;
	}
	sqlite3_finalize(ppStmt);
	return true;
}

Database::Database_Rslt::Database_Rslt(sqlite3_stmt *stmt):
mStmt(stmt), mColummNum(sqlite3_column_count(stmt)), mRc(-1){
}

Database::Database_Rslt::~Database_Rslt(){
	sqlite3_finalize(mStmt);
}

bool Database::Database_Rslt::nextRow(){
	mRc = sqlite3_step(mStmt);
	if (mRc != SQLITE_ROW)
		return false;
	return true;
}

const char* Database::Database_Rslt::getFieldText(int columnIdx) {
	Assert(columnIdx < mColummNum);
	return (const char*)sqlite3_column_text(mStmt, columnIdx);
}

const char* Database::Database_Rslt::getFieldBlob(int columnIdx) {
	Assert(columnIdx < mColummNum);
	const char* data = (const char*)sqlite3_column_blob(mStmt, columnIdx);
	//int size = sqlite3_column_bytes(mStmt, columnIdx);
	return data;
}

shared_ptr<Database::Database_Rslt> Database::runQuery(const std::string &sqlstr) {
	sqlite3_stmt *stmt;
	int rc = sqlite3_prepare(mDb, sqlstr.c_str(), -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		Error << sqlite3_errstr(rc) << "(" << rc << "): " << sqlstr << endl;
		return 0;
	}
	return make_shared<Database_Rslt>(stmt);
}

bool Database::load() {
	ifstream db_file(mName);
	if (db_file.good()) {
Debug<<	"opendb" << endl;
		int rc;
		rc = sqlite3_open("doc_master.db", &mDb);
		if (rc) {
			sqlite3_close(mDb);
			Error << sqlite3_errstr(rc) << "(" << rc << ")" << endl;
			exit(1);
		}
		string ver = getConfig("version");
		Assert(ver >= "0.1");
	}
	else {
		return false;
	}
	return true;
}

bool Database::init() {
	int rc;
	rc = sqlite3_open("doc_master.db", &mDb);
	if (rc) {
		sqlite3_close(mDb);
		Error << sqlite3_errstr(rc) << "(" << rc << ")" << endl;
		exit(1);
	}
	string sqlstr = "create table t_config(key text PRIMARY KEY ASC, value text)";
	rc = runSql(sqlstr);
	Assert(rc);
	sqlstr = "create table t_bitset(word text PRIMARY KEY ASC, bitset blob)";
	rc = runSql(sqlstr);
	Assert(rc);
	sqlstr = "create table t_section(id integer PRIMARY KEY ASC, file_id integer, content text)";
	rc = runSql(sqlstr);
	Assert(rc);
	sqlstr = "create table t_file(id integer PRIMARY KEY ASC, name text, path text)";
	rc = runSql(sqlstr);
	Assert(rc);
	setConfig("version", "0.1");
	setConfig("uid", "0");
	return true;
}

void Database::setConfig(const std::string &key, const std::string &value) {
	string v = getConfig(key);
	string sqlstr;
	if (v == "")
		sqlstr = "insert into t_config values('"+ key + "', '" + value + "')";
	else {
		if (v == value) return;
		sqlstr = "update t_config set value = '" + value + "' where key = '" + key + "'";
	}

	bool rc = runSql(sqlstr);
	Assert(rc);
}


string Database::getConfig(const std::string &key) {
	string sqlstr = "select value from t_config where key='" +key + "'";
	shared_ptr<Database_Rslt> rs = runQuery(sqlstr);
	Assert(rs);
	bool hasMore = rs->nextRow();
	if (!hasMore) return "";
	const char* value = rs->getFieldText(0);
	return string(value);
}

void Database::insertSection(int id, int file_id, const string &section) {
	ostringstream ss;
	ss << "insert into t_section values(" << id << ","
			<< file_id << ",?)";

	sqlite3_stmt *ppStmt = NULL;
	int rc = sqlite3_prepare(mDb, ss.str().data(), -1, &ppStmt, NULL);
	if (rc != SQLITE_OK) {
		Error << sqlite3_errstr(rc) << "(" << rc << "): " << ss.str() << endl;
		Assert(0);
	}
	sqlite3_bind_text(ppStmt, 1, section.data(), section.length(), NULL );
	rc = sqlite3_step(ppStmt);
	if (rc != SQLITE_DONE) {
		sqlite3_finalize(ppStmt);
		Error << sqlite3_errstr(rc) << "(" << rc << "): " << ss.str() << endl;
		Assert(0);
	}
	sqlite3_finalize(ppStmt);
}

string Database::getSection(int id) {
	ostringstream ss;
	ss << "select content from t_section where id=" << id;
	shared_ptr<Database_Rslt> rs = runQuery(ss.str());
	Assert(rs);
	bool hasMore = rs->nextRow();
	if (!hasMore) return "";
	const char* value = rs->getFieldText(0);
	return string(value);
}

int Database::getUid() {
	static int db_uid, num_uid = 0;
	if (num_uid == 0) {
		string s = getConfig("uid");
		db_uid = atoi(s.data());
		num_uid = 1024;
		ostringstream ss;
		ss << db_uid+1024;
		setConfig("uid", ss.str());
	}
	int uid = db_uid;
	db_uid++;
	num_uid--;
	return uid;
}

bool Database::insertBitset(const std::string &key, std::shared_ptr<Bitset> bs) {
	string sqlstr = "select word from t_bitset where word='" + key + "'";
	shared_ptr<Database_Rslt> rs = runQuery(sqlstr);
	Assert(rs);
	if (!rs->nextRow())
		sqlstr = "insert into t_bitset values('" + key + "',?)";
	else
		sqlstr = "update t_bitset set bitset=? where word='" + key + "'";
	sqlite3_stmt *ppStmt = NULL;
	int rc = sqlite3_prepare(mDb, sqlstr.c_str(), -1, &ppStmt, NULL);
	if (rc != SQLITE_OK) {
		Error << sqlite3_errstr(rc) << "(" << rc << "): " << sqlstr << endl;
		return false;
	}
	sqlite3_bind_blob(ppStmt, 1, bs->data(), bs->length(), NULL );
	rc = sqlite3_step(ppStmt);
	if (rc != SQLITE_DONE) {
		sqlite3_finalize(ppStmt);
		Error << sqlite3_errstr(rc) << "(" << rc << "): " << sqlstr << endl;
		return false;
	}
	sqlite3_finalize(ppStmt);
	return true;
}

shared_ptr<Bitset> Database::getBitset(const string &key) {
	string sqlstr = "select bitset from t_bitset where word='" + key + "'";
	shared_ptr<Database_Rslt> rs = runQuery(sqlstr);
	Assert(rs);
	bool hasMore = rs->nextRow();
	if (!hasMore) return 0;
	const char* value = rs->getFieldBlob(0);
	//static char array[eDefualtBitsetSize];
	//Assert(memcmp(array, value, eDefualtBitsetSize) != 0);
	shared_ptr<Bitset> bs = make_shared<Bitset>((char*)value, eDefualtBitsetSize);
	//Assert(memcmp(array, bs->data(), eDefualtBitsetSize) != 0);
	return bs;
}

void Database::beginTransaction() {
	sqlite3_stmt *ppStmt = NULL;
	int rc = sqlite3_prepare(mDb,"begin",-1,&ppStmt,0);
	if (rc != SQLITE_OK) {
		Error << sqlite3_errstr(rc) << "(" << rc << "): begin" << endl;
		Assert(0);
	}
	sqlite3_step(ppStmt);
	if (rc != SQLITE_OK) {
		sqlite3_finalize(ppStmt);
		Error << sqlite3_errstr(rc) << "(" << rc << "): begin" << endl;
		Assert(0);
	}
	sqlite3_finalize(ppStmt);
}

void Database::commitTransaction() {
	sqlite3_stmt *ppStmt = NULL;
	int rc = sqlite3_prepare(mDb,"commit",-1,&ppStmt,0);
	if (rc != SQLITE_OK) {
		Error << sqlite3_errstr(rc) << "(" << rc << "): commit" << endl;
		Assert(0);
	}
	sqlite3_step(ppStmt);
	if (rc != SQLITE_OK) {
		sqlite3_finalize(ppStmt);
		Error << sqlite3_errstr(rc) << "(" << rc << "): commit" << endl;
		Assert(0);
	}
	sqlite3_finalize(ppStmt);
}
