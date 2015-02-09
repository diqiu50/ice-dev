#include <iostream>
#include <algorithm>

using namespace std;

int main()
{
	const int LEN = 17;
	int *nums = new int[LEN];
	for (int i=0; i<LEN; i++)
	{
		nums[i] = rand()%LEN;
	}
	sort(nums, nums+LEN);        
	return 0;
}
