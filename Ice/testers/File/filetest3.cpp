#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

#define FILESIZE  0x3fffffff
#define PAGESIZE  (0x1<<20)
int main()
{
	remove("aa1.txt");
	remove("aa2.txt");

	clock_t c1, c2;
	time_t t1, t2;
	char * buff = new char[PAGESIZE];

	int filefd;
	filefd = open("aa1.txt", O_CREAT|O_RDWR, 00777);
	assert(filefd != -1);
	cout << "create file " << FILESIZE << " bytes" << endl; 
	c1 = clock();
	t1 = time(NULL);
	for (unsigned long long i = 0; i < FILESIZE/PAGESIZE; i++)
	{
		int n =  write(filefd, buff, PAGESIZE);
		if (n == -1)
		{
			perror("failed to write!");	
			assert(0);
		}
	}
	fsync(filefd);
	c2 = clock();
	t2 = time(NULL);
	cout << "file create cuptime: " << (c2-c1)/CLOCKS_PER_SEC << " sec " << endl;
	cout << "file create time: " << (t2-t1) << " sec " << endl;
	return 0;

	FILE * pFile;
	pFile = fopen("aa2.txt", "w");
	cout << "create file " << FILESIZE << " bytes" << endl; 
	//char * buff = new char[PAGESIZE];
	c1 = clock();
	t1 = time(NULL);
	for (unsigned long long i = 0; i < FILESIZE/PAGESIZE; i++)
	{
		int n = fwrite(buff, PAGESIZE, 1, pFile);
		if (n == -1)
		{
			perror("failed to write!");	
			assert(0);
		}
	}
	fflush(pFile);
	c2 = clock();
	t2 = time(NULL);
	cout << "file create cuptime: " << (c2-c1)/CLOCKS_PER_SEC << " sec " << endl;
	cout << "file create time: " << (t2-t1) << " sec " << endl;
	return 0;

	/*
	pFile = fopen("aa2.txt", "w");
	int pagesize = PAGESIZE;
	cout << "write file " << FILESIZE << " bytes, pagesize: " << pagesize << endl; 
	c1 = clock();
	t1 = time(NULL);
	char * buff = new char[PAGESIZE];
	//for (unsigned long long i = 0; i <= FILESIZE; i+= PAGESIZE)
	//for (unsigned long long i = 0; i <= FILESIZE/pagesize; i++)
	for (unsigned long long i = 0; i <= 500; i++)
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
	*/
}
