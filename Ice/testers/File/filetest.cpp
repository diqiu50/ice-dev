#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#include <time.h>
#include <stdio.h>

using namespace std;

#define FILESIZE  0xfffffff
int main()
{
	remove("aa.txt");
	FILE * pFile;
	pFile = fopen("aa.txt", "w");
	cout << "create file " << FILESIZE << " bytes" << endl; 
	clock_t c1 = clock();
	time_t t1 = time(NULL);
	for (unsigned long long i = 0; i <= FILESIZE; i++)
	{
		fputc((int)i, pFile);
	}
	clock_t c2 = clock();
	time_t t2 = time(NULL);
	cout << "file create time: " << (c2-c1)/CLOCKS_PER_SEC << " sec " << endl;
	cout << "file create time: " << (t2-t1) << " sec " << endl;

	pFile = fopen("aa.txt", "r");
	cout << "read file " << FILESIZE << " bytes" << endl; 
	c1 = clock();
	for (unsigned long long i = 0; i <= FILESIZE; i++)
	{
		char a = fgetc(pFile);
	}

	c2 = clock();
	cout << "read time: " << (c2-c1)/CLOCKS_PER_SEC << " sec" << endl;

	fseek(pFile, 0, SEEK_SET);
	c1 = clock();
	char buff[4096];
	for (unsigned long long i = 0; i <= FILESIZE; i+=4096)
	{
		fread(buff, 4096, 1, pFile);
	}

	c2 = clock();
	cout << "read time: " << (c2-c1)/CLOCKS_PER_SEC << " sec" << endl;

	c1 = clock();
	for (unsigned long long i = 0; i <= FILESIZE; i++)
	{
		fseek(pFile, i, SEEK_SET);
		char a = fgetc(pFile);
	}
	c2 = clock();
	cout << "read time(seek): " << (c2-c1)/CLOCKS_PER_SEC << " sec" << endl;

	c1 = clock();
	for (unsigned long long i = 0; i <= FILESIZE; i++)
	{
		fseek(pFile, 0-(i+1), SEEK_END);
		char a = fgetc(pFile);
	}
	c2 = clock();
	cout << "read time(reverse): " << (c2-c1)/CLOCKS_PER_SEC << " sec" << endl;

	fclose(pFile);
	return 0;
	/*
	fstream file1("aa.txt",fstream::out);
	clock_t t1 = clock();
	for (unsigned long long i = 0; i <= FILESIZE; i++)
	{
		file1.put((char)i);
		if (!file1.good())
		{
			cout << "badfile" << endl;
			break;
		}
	}
	clock_t t2 = clock();
	cout << "file create time: " << (t2-t1)/CLOCKS_PER_SEC << " sec " << endl;
	file1.close();

	fstream file("aa.txt", fstream::in);
	t1 = clock();
	for (unsigned long long i = 0; i <= FILESIZE; i++)
	{
		char a;
		//file.seekg(0-(i+1),ios::end);
		//cout << file.tellg() << endl;
		file.get(a);
		//cout << (int)a << endl;
		if (file.eof())
		{
			cout << "badfile" << endl;
			break;
		}
	}
	t2 = clock();
	cout << "read time: " << (t2-t1)/CLOCKS_PER_SEC << " sec" << endl;
	t1 = clock();
	for (unsigned long long i = 0; i <= FILESIZE; i++)
	{
		char a;
		file.seekg(0-(i+1),ios::end);
		//cout << file.tellg() << endl;
		file.get(a);
		//cout << (int)a << endl;
		if (file.eof())
		{
			cout << "badfile" << endl;
			break;
		}
	}
	t2 = clock();
	cout << "read time(reverse): " << (t2-t1)/CLOCKS_PER_SEC << " sec" << endl;
	file.close();
	*/
}
