/*
 * Btree.h
 *
 *  Created on: Sep 1, 2012
 *      Author: Ice
 */

#ifndef BTREE_H_
#define BTREE_H_

#include "BtreeNode.h"

#include <memory>
#include <map>
#include <fstream>
using namespace std;

class Btree {
	shared_ptr<fstream>		mFile;
	shared_ptr<BtreeNode> 	mRoot;
	map<int ,shared_ptr<BtreeNode> >		mNodes;

public:
	explicit Btree(const string &filename);
	virtual ~Btree();
	void insert(const string &key, long value);
	long find(const string &key);
	void erase(const string &key);

private:
	shared_ptr<BtreeNode> findNode(const string &key);
	shared_ptr<BtreeNode> getNode(int nodeId);
	shared_ptr<BtreeNode> splitNode(shared_ptr<BtreeNode>, const string &key);
};

#endif /* BTREE_H_ */
