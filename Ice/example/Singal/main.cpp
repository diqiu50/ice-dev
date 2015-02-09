#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
using namespace std;

void procTimeout(int signal)
{
	printf("%s%d\n", "SIGHANDLER",signal);
	//printf("xxx\n");
}

int main()
{
	signal(SIGALRM, procTimeout);
	itimerval time;
	memset(&time, 0, sizeof(time));
	time.it_value.tv_sec = 5;
	time.it_value.tv_usec = 0;
	time.it_interval.tv_sec = 1;
	time.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL, &time, NULL);
	pause();
	for (int i=0; i< 100; i++)
	{
		procTimeout(-1);
	}
	return 0;
}
