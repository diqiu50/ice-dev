//============================================================================
// Name        : DocumentMaster.cpp
// Author      : Ice
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "WordParser.h"
#include "ConsoleLog.h"
#include "DataBase.h"
#include <iostream>
#include <fstream>
#include <array>
#include <time.h>

using namespace std;

void testICTCLAS_ParagraphProcess();
void insertParagraph(char* sSentence, shared_ptr<Database> db);
void testQusery(string &keywords);

int main(int argc, char** argv) {
	Database::init("doc_master.db");
	shared_ptr<Database> db = Database::getSelf();
	if (!db->load()) {
		//Info << "init db..." << endl;
		db->init();
	}
	testICTCLAS_ParagraphProcess();
	string words = "testing";
	if (argc == 2)
		words = argv[1];
	testQusery(words);
	return 0;
}

void testICTCLAS_ParagraphProcess() {
	Info << "Starting..." << endl;
	int t1 = time(NULL);
	ifstream file("example.txt");
	array<char, 4096> line;
	int i = 0;
	shared_ptr<Database> db = Database::getSelf();
	db->beginTransaction();
	do {
		file.getline((char*)line.data(), line.max_size());
		int length =file.gcount();
		if (length < 30) continue;
		Assert(length <4096);
		i++;
		line[length] = 0;
		if (i>=0)
		{
			//if (i>1) break;
			//Info << "(" << i << ":" << length << ")" << line.data() << endl;
			insertParagraph(line.data(), db);
		}
	}while(!file.eof());
	Bitset::flush();
	db->commitTransaction();
	Info << "Finished(took: " << time(NULL)-t1 << "s)" << endl;
}

void insertParagraph(char* sSentence, shared_ptr<Database> db) {
	int id = db->getUid();
	db->insertSection(id,0, sSentence);
	shared_ptr<Bitset> section_bs = Bitset::fetchBitset("__section");
	section_bs->set(id, true);
	unsigned int nPaLen = strlen(sSentence); // 需要分词的长度
	shared_ptr<WordParser::WordParser_Rslt> rslt =
			WordParser::getSelf().paragraphProcess(sSentence, nPaLen);
	while (rslt->hasMore()) {
		string word = rslt->nextWord();
		Info <<	word << endl;
		shared_ptr<Bitset> word_bs = Bitset::fetchBitset(word);
		word_bs->set(id, true);
	}
	return;
}

void testQusery(string &keywords) {
	shared_ptr<Database> db = Database::getSelf();
	shared_ptr<Bitset> keywords_bs = Bitset::getBitset(keywords);
	vector<int> v;
	if (keywords_bs != NULL) {
		v = keywords_bs->getTureBit(20);
	}

	if (v.empty()) {
		shared_ptr<Bitset> word_bs;
		shared_ptr<WordParser::WordParser_Rslt> rslt =
				WordParser::getSelf().paragraphProcess(keywords.data(), keywords.length());
		while (rslt->hasMore()) {
			string word = rslt->nextWord();
			Info <<	word << endl;
			if (word_bs == NULL) {
				word_bs = Bitset::getBitset(word);
				if (word_bs == NULL) continue;
				word_bs = word_bs->clone();
			}else
				*word_bs &= *Bitset::getBitset(word);
		}
		if (word_bs != NULL) v = word_bs->getTureBit(20);
	}

	ostringstream ss;
	ss << keywords << " (" << v.size() << "):\n";
	for (size_t t=0; t<v.size(); t++) {
		ss << "(" << v[t] << ")" << db->getSection(v[t]) << "\n";
	}
	Info << ss.str() << endl;
}
