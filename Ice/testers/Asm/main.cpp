#include <string.h>
#include <time.h>
#include <stdio.h>
#include <emmintrin.h>


int main()
{
	__m128i a,b;
	a = _mm_setr_epi32(1,2,3,4);
	b = _mm_setr_epi32(2,2,3,4);
	printf("%ld\n", sizeof(a));
	//a1.m128i_i32[1] = 0x1;
	//a1.m128i_i32[2] = 0x2;
	//a1.m128i_i32[4] = 0x3;
	//__m128i b1 = 20;
	__m128i c =  _mm_sub_epi32(a, b);
	c = _mm_setr_epi32(0,0,0,0);
	/*
	char c1[4096];
	char c2[4096];

	clock_t t,t2;
	int i = 100000000;
	t = clock();
	while(i--)
	{
		//memset(c1, 0, 4096);
		//strncpy(c1, c2, 4096);
		strncpy(c1, c2, 4096);
		//memcpy(c1, c2, 4096);
		//memmove(c1, c2, 4096);
	};
	t2 = clock();
	printf("time cost: %d\n" , (int)(t2-t));

	i = 100000000;
	t = clock();
	while(i--)
	{
		//memset(c1, 0, 4096);
		//strncpy(c1, c2, 4096);
		//strncpy(c1, c2, 4096);
		memcpy(c1, c2, 4096);
		//memmove(c1, c2, 4096);
	};
	t2 = clock();
	printf("time cost: %d\n" , (int)(t2-t));

	i = 100000000;
	t = clock();
	while(i--)
	{
		//memset(c1, 0, 4096);
		//strncpy(c1, c2, 4096);
		//strncpy(c1, c2, 4096);
		//memcpy(c1, c2, 4096);
		memmove(c1, c2, 4096);
	};
	t2 = clock();
	printf("time cost: %d\n" , (int)(t2-t));

	i = 100000000;
	t = clock();
	while(i--)
	{
		memset(c1, 0, 4096);
		//strncpy(c1, c2, 4096);
		//strncpy(c1, c2, 4096);
		//memcpy(c1, c2, 4096);
		//memmove(c1, c2, 4096);
	};
	t2 = clock();
	printf("time cost: %d\n" , (int)(t2-t));
	return 0;
	*/
}
