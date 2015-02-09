#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>

#include <iostream>
#include <vector>
#include <set>

using namespace std;

pthread_mutex_t lock;

int thrdfun1();
int thrdfun2();


int main()
{
	pthread_mutex_init(&lock,NULL);
	int ret1;
	pthread_t p_thread1, p_thread2;
	if (0!=(ret1=pthread_create(&p_thread1,NULL, (void* (*)(void *))thrdfun1,NULL))) printf("sdff");
	if (0!=(ret1=pthread_create(&p_thread2,NULL, (void* (*)(void *))thrdfun2,NULL))) printf("sdff");
	while(1)
	{
		sleep(1000);
	}
	pthread_join(p_thread1,NULL);
	pthread_mutex_destroy(&lock);
	return 0;
}

int thrdfun1()
{
	while(1)
	{
		
	pthread_mutex_lock(&lock);
cout << "aaaaaaaaa" << endl;
	sleep(1);
	pthread_mutex_unlock(&lock);
	sleep(1);
	}

	return 1;
}

int thrdfun2()
{
	while(1)
	{
		pthread_mutex_lock(&lock);
cout << "bbbbbbbb" << endl;
	sleep(3);
		pthread_mutex_unlock(&lock);
	sleep(1);
	}
	return 2;
}
