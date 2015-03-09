/*
 * WordPaser.cpp
 *
 *  Created on: 2013-7-28
 *      Author: Ice
 */

#include "WordParser.h"

#include "ConsoleLog.h"
#include <stdio.h>

WordParser WordParser::smParser;
const bool WordParser::WordParser_Rslt::smFilter[100] = {
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //0
				1, 0, 1, 1, 1, 1, 1, 1, 1, 1, //10
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //20
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //30
				1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //40
				1, 1, 1, 1, 1, 0, 1, 1, 1, 1, //50
				1, 0, 1, 1, 1, 1, 1, 1, 1, 1, //60
				1, 1, 1, 1, 1, 1, 1, 1, 0, 1, //70
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //80  ���
				0, 0, 0, 1, 1, 1, 1, 1, 1, 1  //90 93����
		};

WordParser::WordParser() {
	// TODO Auto-generated constructor stub
	bool success = ICTCLAS_Init(); //��ʼ���ִ������
	Assert(success);

   //���ô��Ա�ע��(0 ������������ע����1 ������һ����ע����2 ���������ע����3 ����һ����ע��)
	ICTCLAS_SetPOSmap(2);
	unsigned int nItems=ICTCLAS_ImportUserDictFile("userdict.txt",CODE_TYPE_UNKNOWN);
	Info << "�����" << nItems << "���ʻ�" << endl; //�ļ���ʽ�����û��ֵ�
}

WordParser::~WordParser() {
	// TODO Auto-generated destructor stub
	ICTCLAS_SaveTheUsrDic(); //������ӵ��û��ʻ�,�´�ʹ����Ȼ��Ч
	ICTCLAS_Exit();	//�ͷ���Դ�˳�
}


std::shared_ptr<WordParser::WordParser_Rslt> WordParser::paragraphProcess(
			const char*  pszText,
			int			    iLength,
			eCodeType	codeType){
	int nResultCount;
	LPICTCLAS_RESULT rslt = ICTCLAS_ParagraphProcessA(pszText,iLength,nResultCount,codeType,true);
	std::shared_ptr<WordParser_Rslt> r = std::make_shared<WordParser_Rslt>(pszText, rslt, nResultCount);
	return r;
}

std::string WordParser::WordParser_Rslt::nextWord() {
	//while(!smFilter[mCrt->iPOS]) {
	//	mCrt++;
	//}
	Assert(mCrt-mData < mLen);
	std::string s(mText+mCrt->iStartPos, mCrt->iLength);
	printf("%d %s\t", mCrt->iPOS, mCrt->szPOS);
	mCrt++;
	return s;
}

bool WordParser::WordParser_Rslt::hasMore() {
	do {
		if (mCrt-mData == mLen) return false;
		Assert(mCrt-mData < mLen);
		if (smFilter[mCrt->iPOS]) {
			//ignore english worlds
			if (mCrt->iPOS!=93 || mCrt->iLength>1)
			 return true;
		}
		mCrt++;
	} while(1);
	return true;

	/*
	if (mCrt-mData == mLen) return false;
	while(!smFilter[mCrt->iPOS]) {
		mCrt++;
		if (mCrt-mData == mLen)
			return false;
	}
	return true;
	*/
}
