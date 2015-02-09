
#include "Sort.h"
#include <string.h>
#include <algorithm>
#include <iostream>

using namespace std;

int main()
{
	const int maxlen = 200000000;
	
	//const int maxlen = 2000000;
	int *array = new int[maxlen];
	int *array2 = new int[maxlen];

	for(int i=0; i<maxlen; i++)
	{
		int i = rand() % 4;
		switch(i)
		{
			case 0: array[i] = 1; break;
			case 1: array[i] = 2; break;
			case 2: array[i] = 3; break;
			case 3: array[i] = rand();break;
		}
	}
	memcpy(array, array2, maxlen*4);
	char* nums = (char*)array;
	AosCompareFunPtr comp = new AosCompareFun(4);
	AosSort(nums, nums+maxlen*4, comp);
	//std::sort(array2, array2+maxlen);
	int rslt = memcmp(array, array2, maxlen*4);
	if(rslt != 0)
		cout << "error" << endl;
	else
		cout << "success" << endl;
}
