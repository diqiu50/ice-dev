#include <iostream>
#include <mongo/client/dbclient.h>
#include <time.h>

using namespace std;
using namespace mongo;

int main(int argc, char** argv) {
	DBClientConnection conn;
	try {

		conn.connect("localhost");
		cout << "Connected ok!" << endl;
	} catch(DBException &e) {
		cout << e.what() << endl;
	}

	time_t t1 = time(NULL);
	for (int i=0; i< 5000000; i++)
	{
		char buff[20];
		strcpy(buff, "mongo_");
		sprintf(&buff[6], "%d", getpid()*i);
		BSONObj p = BSONObjBuilder().append("name", buff).append("age", i).
			append("num", "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx").obj();
		conn.insert("mongodb.test", p);
	}
	cout << "time cost: " << time(NULL)-t1 << " second."  << endl;
	return 0;
}
