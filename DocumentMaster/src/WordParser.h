/*
 * WordPaser.h
 *
 *  Created on: 2013-7-28
 *      Author: Ice
 */

#ifndef WORDPASER_H_
#define WORDPASER_H_

#include <Windows.h>
#include <memory>
#include <string>
#include "ICTCLAS50.h"
#include <stdio.h>

class WordParser {

public:
	class WordParser_Rslt {
		const static bool smFilter[100];
		const char* mText;
		const LPICTCLAS_RESULT mData;
		const int mLen;
		LPICTCLAS_RESULT mCrt;
	public:
		WordParser_Rslt(const char* text,
				const LPICTCLAS_RESULT data, const int len):mText(text), mData(data), mLen(len), mCrt(data){}
		~WordParser_Rslt() {ICTCLAS_ResultFree(mData);}
		std::string nextWord();
		bool hasMore();
	};

private:
	static WordParser smParser;
private:
	WordParser();
	virtual ~WordParser();

public:
	std::shared_ptr<WordParser_Rslt> paragraphProcess(
			const char*  pszText,
			int			    iLength,
			eCodeType	codeType=CODE_TYPE_UNKNOWN);
	static WordParser& getSelf(){return smParser;}
};

#endif /* WORDPASER_H_ */
