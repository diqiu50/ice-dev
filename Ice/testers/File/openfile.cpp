#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;

int main()
{
	vector<FILE*> files;
	for (int i=0; i<99990; i++)
	{
		FILE* ff = fopen("openfile.cpp", "a");
		if (ff == 0)
			cerr << "erro" << endl;
		files.push_back(ff);
	}
	cout << "hello" << endl;
	sleep(10000);
}
