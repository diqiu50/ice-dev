/*
 * BitMap.h
 *
 *  Created on: 2013-7-28
 *      Author: Ice
 */

#ifndef BITMAP_H_
#define BITMAP_H_

#include "LRUCache.hpp"

#include <vector>
#include <memory>
#include <string>

const int eDefualtBitsetSize = 10000;

class Bitset {
	char* mData;
	int mDataLen;
	static unsigned char smBitArray[];
	static LRUCache<std::string, shared_ptr<Bitset>, 4096> smBitsets;

public:
	Bitset();
	Bitset(char* data, int datalen);
	virtual ~Bitset();
	static std::shared_ptr<Bitset> fetchBitset(const std::string &word);
	static std::shared_ptr<Bitset> getBitset(const std::string &word);
	static void flush();

	char* data() {return mData;}
	int length() {return mDataLen;}
	bool get(int idx);
	void set(int idx, bool value);
	std::vector<int> getTureBit(int maxnum);
	Bitset& operator &=(const Bitset& bitset);
	Bitset& operator |=(const Bitset& bitset);
	shared_ptr<Bitset> clone();
};

#endif /* BITMAP_H_ */
