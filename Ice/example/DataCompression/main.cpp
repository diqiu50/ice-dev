#include <iostream>
#include <zlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

using namespace std;

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

const int ARRAYSIZE = 1000000;

int main()
{
	Array* array = new Array[ARRAYSIZE];
	clock_t t = clock();
	for (int i=0; i<ARRAYSIZE; i++)
	{
		array[i].value = rand();
		char buff[25];
		randstr(buff, 24);
		strcpy(array[i].name, buff);
	}
	cout << "create buff time: " << clock()-t << endl;
	uLong len =  compressBound(sizeof(Array)*ARRAYSIZE);
	unsigned char * dest = new unsigned char[len];
	t = clock();
	int r = compress2(dest, &len, (unsigned char*)array, sizeof(Array)*ARRAYSIZE, 2);
	if (r == Z_OK)
	{
		cout << "compress time: " << clock()-t << endl;
		cout << "source size :" << sizeof(Array)*ARRAYSIZE << "\tcompress size :" << len <<
			"\tcompress rate :" << 1-(double)len/(sizeof(Array)*ARRAYSIZE) << endl;
	}
	else
	{
		if (r == Z_MEM_ERROR)
			cout << "not enough memery!" << endl;
		else if (r == Z_BUF_ERROR)
			cout << "buff not enough!" << endl;
		else
			cout << "failed to compress!" << endl;
	}

	t = clock();
	unsigned long len2 = sizeof(Array)*ARRAYSIZE;
	r = uncompress ((unsigned char*)array, &len2, dest, len);
	if (r == Z_OK)
	{
		cout << "uncompress time: " << clock()-t << endl;
	}
	else
	{
		if (r == Z_MEM_ERROR)
			cout << "not enough memery!" << endl;
		else if (r == Z_BUF_ERROR)
			cout << "buff not enough!" << endl;
		else
			cout << "failed to compress!" << endl;
	}

	delete[] array;
	return 0;
}


//g++ main.cpp -O3 -lz
