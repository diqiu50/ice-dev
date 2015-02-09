#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
using namespace std;

bool stop_1=false;
bool pause_1=false;

void procTimeout(int signal)
{
	printf("%s%d\n", "SIGHANDLER:",signal);
	if (signal == SIGUSR1)
	{
		pause_1 = true;
		for (int i=0; i< 100000; i++)
		{
			//procTimeout(-1);
			if(!pause_1)
				break;
			sleep(3);
			printf("Pause\n");
		}
	}
	else
	{
		if(pause_1)
		{
			pause_1 = false;
		}
		else
		{
			stop_1=true;
		}
	}

	//printf("xxx\n");
}

int main()
{
	printf("%s%d\n", "Pid:",getpid());
	signal(SIGALRM, procTimeout);
	signal(SIGUSR1, procTimeout);
	/*
	itimerval time;
	memset(&time, 0, sizeof(time));
	time.it_value.tv_sec = 20;
	time.it_value.tv_usec = 0;
	time.it_interval.tv_sec = 0;
	time.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL, &time, NULL);
	*/
	//pause();
	for (int i=0; i< 100000; i++)
	{
		//procTimeout(-1);
		if(stop_1)
			break;
		sleep(3);
		printf("Run\n");
	}
	return 0;
}
