/*
 * Datebase.h
 *
 *  Created on: 2014Äê3ÔÂ7ÈÕ
 *      Author: Ice
 */

#ifndef DATEBASE_H_
#define DATEBASE_H_

#include "Bitset.h"
#include "sqlite3.h"
#include <string>
#include <memory>

class Database {
	std::string mName;
	sqlite3 *mDb;
	static std::shared_ptr<Database> smDb;
public:
	class Database_Rslt {
		sqlite3_stmt *mStmt;
		int mColummNum;
		int mRc;
	public:
		Database_Rslt(sqlite3_stmt *stmt);
		~Database_Rslt();
		const char* getFieldText(int columnIdx);
		const char* getFieldBlob(int columnIdx);
		bool nextRow();
	};

	Database(const std::string &dbname);
	virtual ~Database();
	static void init(const std::string &dbname);
	static std::shared_ptr<Database> getSelf();
public:
	bool load();
	bool init();
	void setConfig(const std::string &key, const std::string &value);
	std::string getConfig(const std::string &key);
	void insertSection(int id, int fileid, const std::string &section);
	string getSection(int id);
	int getUid();
	bool insertBitset(const std::string &key, std::shared_ptr<Bitset> bs);
	std::shared_ptr<Bitset> getBitset(const std::string &key);
	void beginTransaction();
	void commitTransaction();
private:
	bool runSql(const std::string &sqlstr);
	std::shared_ptr<Database_Rslt> runQuery(const std::string &sqlstr);

};

#endif /* DATEBASE_H_ */
