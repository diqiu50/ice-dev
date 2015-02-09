#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define FILESIZE  0x3fffffff
#define PAGESIZE  0x1<<20
int main()
{
	remove("aa1.txt");
	remove("aa2.txt");
	FILE * pFile;
	clock_t c1, c2;
	time_t t1, t2;
	/*
	pFile = fopen("aa1.txt", "w");
	cout << "create file " << FILESIZE << " bytes" << endl; 
	c1 = clock();
	t1 = time(NULL);
	//for (unsigned long long i = 0; i <= FILESIZE; i++)
	for (unsigned long long i = 0; i <= 0; i++)
	{
		fputc((int)i, pFile);
	}
	c2 = clock();
	t2 = time(NULL);
	cout << "file create cuptime: " << (c2-c1)/CLOCKS_PER_SEC << " sec " << endl;
	cout << "file create time: " << (t2-t1) << " sec " << endl;
	*/

	pFile = fopen("aa2.txt", "w");
	int pagesize = PAGESIZE;
	cout << "write file " << FILESIZE << " bytes, pagesize: " << pagesize << endl; 
	c1 = clock();
	t1 = time(NULL);
	char * buff = new char[PAGESIZE];
	//for (unsigned long long i = 0; i <= FILESIZE; i+= PAGESIZE)
	//for (unsigned long long i = 0; i <= FILESIZE/pagesize; i++)
	for (unsigned long long i = 0; i <= 5000; i++)
	{
		int seekpos = rand()%(FILESIZE/pagesize)*pagesize;
		//cout << i << "," << seekpos << endl;
		fseek(pFile, seekpos, SEEK_SET);
		fwrite(buff, PAGESIZE, 1, pFile);
	}
	c2 = clock();
	t2 = time(NULL);
	cout << "file create cuptime: " << (c2-c1)/CLOCKS_PER_SEC << " sec " << endl;
	cout << "file create time: " << (t2-t1) << " sec " << endl;
}
