#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "b.c"

int main(int argc, char **argv)
{
	int num = 200000;
	char *a = new char[num];
	char *b = new char[num];
	for (int i=0; i<num; i++)
	{
		a[i]= rand();
		b[i] = a[i];
	}
	b[num-1] = 0;
	printf("start!\n");
	clock_t c1 = clock();
	for (int i=0; i< num; i++)
	{
		if (a[i] != b[i])
		{
			printf("miss match!\n");
			break;
		}
	}
	clock_t c2 = clock();
	printf("%d\n",int(c2-c1));


	c1 = clock();
	int d= 100;
	int r = 0;
	while(d--)
	{
		//r = memcmp(a, b, num);
		memcmp(a, b, num);
	}
	c2 = clock();
	printf("%d\n",r);
	printf("%d\n",int(c2-c1));
	//extern int a;
	//extern void ap();
	//printf("%d\n", memcmp("1110", "1111", 4));
	//const char *p = "abc";
	//const char *b = &p[3];
	//ap();
}
