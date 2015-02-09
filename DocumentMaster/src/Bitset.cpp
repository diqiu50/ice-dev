/*
 * BitMap.cpp
 *
 *  Created on: 2013-7-28
 *      Author: Ice
 */

#include "Bitset.h"

#include "Database.h"
#include "ConsoleLog.h"
#include <string.h>

using namespace std;
struct BitsetGen {
	shared_ptr<Bitset> getData(const string &word) {
		shared_ptr<Bitset> bitset = Database::getSelf()->getBitset(word);
		//if (bitset == NULL) bitset = make_shared<Bitset>();
		return bitset;
	}
	shared_ptr<Bitset> getData(const string &word, const string &oldkey, shared_ptr<Bitset> bs) {
		if (bs !=NULL) Database::getSelf()->insertBitset(oldkey, bs);
		shared_ptr<Bitset> bitset = Database::getSelf()->getBitset(word);
		//if (bitset == NULL) bitset = make_shared<Bitset>();
		return bitset;
	}
};

unsigned char Bitset::smBitArray[] = {0b10000000, 0b01000000, 0b00100000, 0b00010000,
		0b00001000, 0b00000100, 0b00000010, 0b00000001};

LRUCache<std::string, shared_ptr<Bitset>, 4096> Bitset::smBitsets;

Bitset::Bitset():mData(new char[eDefualtBitsetSize]), mDataLen(eDefualtBitsetSize) {
	memset(mData, 0, mDataLen);
}

Bitset::Bitset(char* data, int datalen):mData(new char[eDefualtBitsetSize]), mDataLen(eDefualtBitsetSize) {
	memset(mData, 0, mDataLen);
	memcpy(mData, data, datalen);
}

Bitset::~Bitset() {
	delete[] mData;
}

shared_ptr<Bitset> Bitset::fetchBitset(const std::string &word){
	static BitsetGen bsg;
	shared_ptr<Bitset>& bitset = smBitsets.get(word, bsg);
	if (bitset == NULL) {
		shared_ptr<Bitset> tmp = make_shared<Bitset>();
		bitset.swap(tmp);
	}
	return bitset;
}

shared_ptr<Bitset> Bitset::getBitset(const std::string &word){
	static BitsetGen bsg;
	return smBitsets.get(word, bsg);
}

void Bitset::flush() {
	Info << "Cache hit rate: " << smBitsets.hitRate() << endl;
	list<pair<string, shared_ptr<Bitset> > > bss = smBitsets.copy();
	for (list<pair<string, shared_ptr<Bitset> > >::iterator itr=bss.begin(); itr!=bss.end(); ++itr) {
		Database::getSelf()->insertBitset(itr->first, itr->second);
	}
}

bool Bitset::get(int idx) {
	int byte_idx = idx/8;
	int byte_pos = idx%8;
	return smBitArray[byte_pos] & mData[byte_idx];
}

void Bitset::set(int idx, bool value) {
	int byte_idx = idx/8;
	int byte_pos = idx%8;
	if (value)
		mData[byte_idx] |= smBitArray[byte_pos];
	else
		mData[byte_idx] &= ~smBitArray[byte_pos];
}

vector<int> Bitset::getTureBit(int maxnum) {
	vector<int> set;
	for (int i=0; i<mDataLen; i++) {
		if (mData[i]) {
			for (int j=0; j<8; j++) {
				if (mData[i]&smBitArray[j]) {
					set.push_back(i*8+j);
					if (set.size()==(size_t)maxnum)
						return set;
				}
			}
		}
	}
	return set;
}

Bitset& Bitset::operator &=(const Bitset& bitset) {
	for (int i=0; i<mDataLen; i++) {
		mData[i] &= bitset.mData[i];
	}
	return *this;
}

Bitset& Bitset::operator |=(const Bitset& bitset) {
	for (int i=0; i<mDataLen; i++) {
		mData[i] |= bitset.mData[i];
	}
	return *this;
}

shared_ptr<Bitset> Bitset::clone() {
	return make_shared<Bitset>(mData, mDataLen);
}
