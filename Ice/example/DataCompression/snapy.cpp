#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "snappy.h"

using namespace std;
using namespace snappy;

struct Array
{
	char name[32];
	long long value;
};

char sg[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 
			 'h', 'i', 'j', 'k', 'l', 'm', 'n', 
			 'o', 'p', 'q', 'r', 's', 't', 'u',
			 'v', 'w', 'x', 'y', 'z'};

void randstr(char *buff, int maxlen)
{
	int len = rand()%maxlen;
	for (int i=0; i<len; i++)
	{
		buff[i] = sg[rand()%26];
	}
	buff[len] = 0;
}

const int ARRAYSIZE = 20000000;

int main()
{
	Array* array = new Array[ARRAYSIZE];
	clock_t t = clock();
	for (int i=0; i<ARRAYSIZE; i++)
	{
		array[i].value = rand();
		//array[i].value = 0;
		char buff[25];
		//bzero(buff, 25);
		//buff[0] = 'a';
		////buff[1] = '0';
		randstr(buff, 24);
		strcpy(array[i].name, buff);
	}
	cout << "create buff time: " << clock()-t << endl;
	size_t len =  MaxCompressedLength(sizeof(Array)*ARRAYSIZE);
	char * dest = new char[len];
	t = clock();
	//int r = compress2(dest, &len, (unsigned char*)array, sizeof(Array)*ARRAYSIZE, 2);
	int r = 0;
  	RawCompress((char*)array, sizeof(Array)*ARRAYSIZE, dest, &len);
	if (r == 0)
	{
		cout << "compress time: " << clock()-t << endl;
		cout << "source size :" << sizeof(Array)*ARRAYSIZE << "\tcompress size :" << len <<
			"\tcompress rate :" << 1-(double)len/(sizeof(Array)*ARRAYSIZE) << endl;
	}
	else
	{
		cout << "compress failed!" << endl;
	}

	t = clock();
	size_t len2 = sizeof(Array)*ARRAYSIZE;
	//r = uncompress ((unsigned char*)array, &len2, dest, len);
  	RawUncompress(dest, len, (char*)array);
	if (r == 0)
	{
		cout << "uncompress time: " << clock()-t << endl;
	}
	else
	{
		cout << "failed to compress!" << endl;
	}

	delete[] array;
	return 0;
}


//g++ main.cpp -O3 -lz
