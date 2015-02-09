#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <set>
#include <iostream>
#include <list>
#include <vector>
#include <algorithm>

using namespace std;


int main(int argv, char **argc)
{
	
	// testing for the memery usage of set.
	/*
	multiset<string> a;
	for (int i=0;i<10000000; i++)
		a.insert("hello wordxxxxxxxxxxxxx");
	cout << "finished" << endl;
	//sleep(10000000);
	*/
	


	//testing performance between strcpy and memcpy
	/*
	char buff[128];
	clock_t t1 = clock();
	for (int i=0; i<500000000; i++)
	{
		strcpy(buff, "hello");
	}
	cout << clock()-t1 << endl;
	t1 = clock();
	for (int i=0; i<500000000; i++)
	{
		//strcpy(buff, "hello");
		memcpy(buff, "hello", 6);
	}
	cout << clock()-t1 << endl;

	char* buff = new char[10000000];
	for (int i=0; i<1000000; i++)
	{
		buff[i] = (char)i;
	}
	*/


	//testing performance between memrycpy and array assignment.
	/*
	char* buff2 = new char[10000000];
	clock_t t1 = clock();
	for (int j=0; j< 1000; j++)
	{
		for (int i=0; i<1000000; i++)
		{
			buff2[i] = buff[i];
		}
	}
	cout << clock()-t1 << endl;

	t1 = clock();
	for (int j=0; j< 1000; j++)
		memcpy(buff, buff2, 1000000);
	cout << clock()-t1 << endl;

	delete buff;
	delete buff2;
	*/
}




