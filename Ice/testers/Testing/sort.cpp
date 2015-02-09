#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <time.h>

using namespace std;

inline int cmp(const void *l,  const void *r)
{
	return *(int*)l - *(int*)r;
}

int main()
{
	const int LEN = 10;
	int *nums = new int[LEN];
	for (int i=0; i<LEN; i++)
	{
		nums[i] = rand()%LEN;
	}
	sort(nums, nums+LEN);
	return 0;

	/*
	int *nums2 = new int[LEN];
	memcpy(nums2, nums, LEN*4);
	int *nums3 = new int[LEN];
	memcpy(nums3, nums, LEN*4);


	clock_t t = clock();
	stable_sort(nums2, nums2+LEN);
	cout << "stable sort:" << clock() - t << endl;

	t = clock();
	sort(nums3, nums3+LEN);
	cout << "quick sort :" << clock() - t << endl;

	t = clock();
	qsort(nums, LEN, sizeof(int), cmp);
	cout << "cquick sort :" << clock() - t << endl;
	return 0;
	*/
}
