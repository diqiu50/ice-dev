#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <time.h>

using namespace std;

int main()
{
	time_t t = time(0);
	for (int i = 0; i<10000000; i++)
	{
		chdir("/home/ice/AOS/src");
	}
	cout << time(0)-t << endl;

	t = time(0);
	for (int i = 0; i<100000; i++)
	{
		//chdir("/home/ice/AOS/src");
		system("cd /home/ice/AOS/src");
	}
	cout << time(0)-t << endl;

}
