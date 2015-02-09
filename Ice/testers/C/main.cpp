
#include <iostream>
#include <time.h>
#include <stdio.h>

using namespace std;

int main(int argc, char **argv)
{
	unsigned long l = 0x12345678;
	l=l;
	printf("0x%x\n", 0xffffff);
	return 0;
	int t1 = clock();
	int b;
	for (int i=0; i<1; i++)
	{
		for (int i=0; i<1000000000; i++)
		{
			b += i*b + b/238;
		}
	}
	int t2 = clock();

	int  timed = t2 - t1;
	cout << timed << ":" << b << endl;
	return 0;
}
