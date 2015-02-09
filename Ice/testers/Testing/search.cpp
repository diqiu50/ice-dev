#include <iostream>
using namespace std;

int main() {
	int a[10] = {0, 1, 2, 3, 4, 5, 5, 5, 8, 9};

	int len = 10;
	int half = 0;
	int *mid = 0;
	int *low = a;
	while(len>0)
	{
		half = len >> 1;
		mid = low;
		mid += half;
		if (*mid <= 5)
		{
			low = mid;
			low++;
			len -= half+1;
		}
		else
		{
			len = half;
		}

	}
	cout << *low << endl;
	return 0;
}
