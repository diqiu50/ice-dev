//============================================================================
// Name        : MyTester.cpp
// Author      : Ice
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<bitset>
#include <string.h>

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

const int num = 10000000;
//const int num = 1000000;

void test1(u64*);
void test2(u64*);
void test3(u64*);
u64 test4();

int main()
{
	u64* bitmap = new u64[num];
	//Ëæ»úÉèÖÃÄÚ´æ 0 £¬1
	for (int i= rand()%num; i<num;i++)
		memset(bitmap+rand()%num,rand()%255, sizeof(u64));
	//memset(bitmap, 3, sizeof(u64));
	std::cout << "Starting ..." << std::endl;
	u64 t = clock();
	test1(bitmap);
	std::cout << "Test 1: " << clock()-t << "ms" << std::endl;
	t = clock();
	test2(bitmap);
	std::cout << "Test 2: " << clock()-t << "ms" << std::endl;
	t = clock();
	test3(bitmap);
	std::cout << "Test 3: " << clock()-t << "ms" << std::endl;
	delete[] bitmap;
	t = test4();
	std::cout << "Test 4: " << clock()-t << "ms" << std::endl;
	return 0;
}

void test1(u64* bitmap)
{
	int total = 0;
	u32* bit32;
	u16* bit16;
	u8* bit8;
	u8 temp;
	for(int i = 0; i<num; i++)
	{
		if (bitmap[i] == 0) continue;

		bit32 =  (u32*)&bitmap[i];
		for(int j=0; j<2; j++)
		{
			if (bit32[j]==0) continue;

			bit16 =  (u16*)&bit32[j];
			for(int k=0; k<2; k++)
			{
				if (bit16[k]==0) continue;

				bit8 = (u8*)&bit16[k];
				for (int m=0; m<2 ;m++)
				{
					temp = bit8[m];
					/*while(temp)
					{
						if(temp&1)total++;
						temp =  temp >> 1;
					}*/
					temp = (temp&0x55) + ((temp>>1)&0x55);
					temp = (temp&0x33) + ((temp>>2)&0x33);
					temp = (temp&0x0f) + ((temp>>4)&0x0f);
					total+=temp;
				}
			}
		}
	}
	std::cout << "total " << total << std::endl;
}

void test2(u64* bitmap)
{
	int total = 0;
	u8* bit8 = (u8*)bitmap;
	u8 a[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
	for(int i = 0; i<(int)(num*sizeof(u64)); i++)
	{
		for(int j=0; j<8; j++)
		{
			if ((bit8[i]&a[j]))
				total++;
		}
	}
	std::cout << "total " << total << std::endl;
}

void test3(u64* bitmap)
{
	u64 total = 0;
	u64 temp;
	u64 a[6] = {0x55555555, 0x33333333, 0x0f0f0f0f, 0x00ff00ff ,0x0000ffff, 0};
	a[0] = (a[0] << 32) +  0x55555555;
	a[1] = (a[1] << 32) +  0x33333333;
	a[2] = (a[2] << 32) +  0x0f0f0f0f;
	a[3] = (a[3] << 32) +  0x00ff00ff;
	a[4] = (a[4] << 32) +  0x0000ffff;
	a[5] = (a[5] << 32) +  0xffffffff;
	for (int i=0; i<num; i++)
	{
		temp = bitmap[i];
		temp = (temp&a[0]) + ((temp>>1)&a[0]);
		temp = (temp&a[1]) + ((temp>>2)&a[1]);
		temp = (temp&a[2]) + ((temp>>4)&a[2]);
		temp = (temp&a[3]) + ((temp>>8)&a[3]);
		temp = (temp&a[4]) + ((temp>>16)&a[4]);
		temp = (temp&a[5]) + ((temp>>32)&a[5]);

		total+=temp;
	}
	std::cout << "total " << total << std::endl;
}

u64 test4()
{
	std::bitset<sizeof(u64)*num> *bs = new std::bitset<sizeof(u64)*num>;
	int j;
	for(int i=0; i<(int)bs->size(); i++)
	{
		j = rand()%3;
		if (j)
			(*bs)[i] = 0;
		else
			(*bs)[i] = 1;
	}
	u64 t = clock();
	std::cout << "total " << bs->count() << std::endl;
	return t;
}
