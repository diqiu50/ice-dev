/*
 * BtreeNode.cpp
 *
 *  Created on: Sep 1, 2012
 *      Author: Ice
 */

#include "BtreeNode.h"

#include "ConsoleLog.h"

BtreeNode::BtreeNode(int nodeId, shared_ptr<char> buff) : mNodeId(nodeId), mBuff(buff), mSize(0) {
	mSize = *(int*)&mBuff.get()[eNodeSizePos];
	Assert(mSize < csMaxSize)
	mEntries = (Entry*)(buff.get() + sizeof(BtreeNode));
}

BtreeNode::~BtreeNode() {
}

int BtreeNode::findNodeId(const string &key) const {
	return 0;
}

bool BtreeNode::insert(const string &key, long value) {
	//findPosition(key);
	if (mSize == 0) {
		strcpy(mEntries[0].key, key.data());
		mEntries[0].nodeid = -1;
		mEntries[0].value = value;
	}
	return false;
}
