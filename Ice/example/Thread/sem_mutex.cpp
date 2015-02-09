#include <semaphore.h>
#include <iostream>
#include <pthread.h>

using namespace std;
sem_t sem;

void* test(void *)
{
	int i=0;
	do
	{
		//sleep(3);
		//cout << "p" << 3-i << endl;
		//sleep(1);
		//sem_post(&sem);
		cout << "before wait" << endl;
		sem_wait(&sem);
		cout << "after wait" << endl;

	}while(1);
	//cout << "pf" << endl;
}

int main()
{
	pthread_t t, t1, t2;
	pthread_create(&t, NULL, test, NULL);
	/*
	pthread_create(&t1, NULL, test, NULL);
	pthread_create(&t2, NULL, test, NULL);
	sleep(1);
	*/
	/*
	pthread_cancel(t);
	cout << "canceled" << endl;
	*/

	/*
	pthread_mutex_t lock;
	pthread_mutex_init(&lock, NULL);
	pthread_mutex_lock(&lock);
	pthread_mutex_unlock(&lock);
	pthread_mutex_destroy(&lock);
	*/


	sem_init(&sem, 0, 1);
	//int rslt = sem_wait(&sem);
	//rslt = sem_post(&sem);

	sem_destroy(&sem);


	pthread_join(t, NULL);
	return 0;
}
