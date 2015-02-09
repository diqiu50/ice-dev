#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostream>
using namespace std;

int main()
{
	int len = 100000;
	char **mem = (char**)malloc(sizeof(char*)*len);
	//char* mem[len];
	memset(mem, 0, sizeof(char*)*len);
	char *a = 0;
	long long total = 0;
	time_t t = time(NULL);
	do
	{
		int i = rand()%10;
		int size = 0;
		if (i < 5)
		{
			size = i%4000+1;
		}
		else if (i<7)
		{
			size = i%10000+ 2000;
		}
		else if (i<9)
		{
			size = i%100000+ 10000;
		}
		else
		{
			size = i%10000000+ 100000;
		}
		if (total >= len)
		{
			free(mem[total%len]);
		}
		a = (char*)malloc(size);
		mem[total%len] = a;
	
		total++;

		if (total%0xffffff== 0)
			cout << "total " << total << " (" << total/(time(NULL)-t) << "/sec)" << endl;
		
	}while(1);
	return 0;
}
