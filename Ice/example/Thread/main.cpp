#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <list>
#include <set>
#include <vector>
#include <iostream>

using namespace std;

vector<int> sglist;
pthread_mutex_t lock;

int thrdfun()
{
	while(1)
	{
		int i=rand()%100;
		pthread_mutex_lock(&lock);
		while(i--)
		{
		}
		pthread_mutex_unlock(&lock);
	}
	
}
int thrdfun2()
{
	int j = 0;
	while(0)
	{
		j++;
		sglist.push_back(j);
	}
	return 1;
}
int main()
{
	for (int i = 0; i<1000; i++)
	{
		sglist.push_back(i);
	}
	int i=0;
	int rc=0;
	int ret1;
	pthread_mutex_init(&lock, NULL);
	pthread_t p_thread1, p_thread2,p_thread3,p_thread4,p_thread5,p_thread6;
	printf("start...");
	if (0!=(ret1=pthread_create(&p_thread1,NULL, (void* (*)(void *))thrdfun,NULL))) printf("sdff");
	if (0!=(ret1=pthread_create(&p_thread2,NULL, (void* (*)(void *))thrdfun,NULL))) printf("sdff");
	if (0!=(ret1=pthread_create(&p_thread3,NULL, (void* (*)(void *))thrdfun,NULL))) printf("sdff");
	if (0!=(ret1=pthread_create(&p_thread4,NULL, (void* (*)(void *))thrdfun,NULL))) printf("sdff");
	if (0!=(ret1=pthread_create(&p_thread5,NULL, (void* (*)(void *))thrdfun,NULL))) printf("sdff");
	if (0!=(ret1=pthread_create(&p_thread6,NULL, (void* (*)(void *))thrdfun,NULL))) printf("sdff");
	while(1)
	{
		i++;
		sleep(1);
	}

	pthread_join(p_thread1,NULL);
	pthread_mutex_destroy(&lock);
	return 0;
}
