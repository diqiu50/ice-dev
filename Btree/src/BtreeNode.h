/*
 * BtreeNode.h
 *
 *  Created on: Sep 1, 2012
 *      Author: Ice
 */

#ifndef BTREENODE_H_
#define BTREENODE_H_

#include <memory>
#include <string.h>
#include <string>
using namespace std;

class BtreeNode {
public:
	enum {
		eNodeIdPos = 0,
		eNodeSizePos = sizeof(int),
		eNodeSize = 4096
	};
	struct Entry {
		char key[128];
		long value;
		int	 nodeid;
	};

	static const int csMaxSize = eNodeSize/sizeof(Entry) - 1;
private:
	const int 	mNodeId;
	const shared_ptr<char> mBuff;
	int mSize;
	Entry* mEntries;

public:
	BtreeNode(int nodeId, shared_ptr<char> buff);
	~BtreeNode();
	int findNodeId(const string &key) const;
	int getNodeId() const {return mNodeId;}
	bool insert(const string &key, long value);
	long find(const string &key);
	void erase(const string &key);
};

#endif /* BTREENODE_H_ */
