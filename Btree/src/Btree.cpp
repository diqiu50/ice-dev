/*
 * Btree.cpp
 *
 *  Created on: Sep 1, 2012
 *      Author: Ice
 */

#include "Btree.h"

#include "ConsoleLog.h"

Btree::Btree(const string &filename) :
		mFile(shared_ptr<fstream>(new fstream(filename, ios::in | ios::out))) {
	shared_ptr<char> buff(new char[BtreeNode::eNodeSize]);
	int nodeId = 0;
	if (!mFile->is_open()) {
		mFile = shared_ptr<fstream>(
				new fstream(filename, ios::in | ios::out | ios::app));

		Assert(mFile->is_open());
	} else {
		mFile->read(buff.get(), BtreeNode::eNodeSize);
		if (mFile->gcount() != BtreeNode::eNodeSize)
		{
			memset(buff.get(), 0, BtreeNode::eNodeSize);
		}
	}

	mRoot = shared_ptr<BtreeNode>(new BtreeNode(nodeId, buff));
	Assert(nodeId == mRoot->getNodeId());
}

Btree::~Btree() {
}

void Btree::insert(const string &key, long value) {
	shared_ptr<BtreeNode> node = findNode(key);
	Assert(node != NULL);
	bool rslt = node->insert(key, value);
	if (!rslt) {
		node = splitNode(node, key);
		Assert(node == NULL);
		rslt = node->insert(key, value);
		Assert(rslt);
	}
}

shared_ptr<BtreeNode> Btree::findNode(const string &key) {
	shared_ptr<BtreeNode> node = mRoot;
	Assert(node != NULL);
	while (node) {
		long nodeId = node->findNodeId(key);
		if (nodeId == node->getNodeId()) {
			break;
		} else {
			node = getNode(nodeId);
			Assert(node == NULL);
		}
	}
	return node;
}

shared_ptr<BtreeNode> Btree::getNode(int nodeId) {
	shared_ptr<BtreeNode> node;
	map<int, shared_ptr<BtreeNode> >::iterator itr = mNodes.find(nodeId);
	if (itr != mNodes.end()) {
		node = itr->second;
	} else {
		shared_ptr<char> buff(new char[BtreeNode::eNodeSize]);
		mFile->seekg(nodeId * BtreeNode::eNodeSize);
		mFile->read(buff.get(), BtreeNode::eNodeSize);
		int id = *(int*) buff.get();
		Assert(id == nodeId);
		node = shared_ptr<BtreeNode>(new BtreeNode(nodeId, buff));
	}
	return node;
}

shared_ptr<BtreeNode> Btree::splitNode(shared_ptr<BtreeNode>,
		const string &key) {

	return 0;
}
