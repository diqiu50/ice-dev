/*
 * main.cpp
 *
 *  Created on: 2012-5-14
 *      Author: Ice
 */


#include <iostream>
#include <memory>
#include <time.h>
#include <pthread.h>

using namespace std;


static inline unsigned long mktime1(unsigned int year, unsigned int mon,
		unsigned int day, unsigned int hour, unsigned int min,
		unsigned int sec) {
	if (0 >= (int) (mon -= 2)) { /* 1..12 -> 11,12,1..10 */
		mon += 12; /* Puts Feb last since it has leap day */
		year -= 1;
	}

	return ((((unsigned long) (year / 4 - year / 100 + year / 400
			+ 367 * mon / 12 + day) + year * 365 - 719499) * 24 + hour /* now have hours */
	) * 60 + min /* now have minutes */
	) * 60 + sec; /* finally seconds */
}

time_t t1;
unsigned long t2;
struct tm tm1 = {0, 0, 8, 1, 0, 100} ;


void* test(void* data)
{

	/*
	for (int i=0; i< 10000000; i++)
		t1 = mktime(&tm1);
	*/
	for (int i=0; i< 10000000; i++)
		t2 = mktime1(2000, 1, 1, 0, 0, 0);
	return 0;
}




int main(int argc, char **argv)
{
	clock_t c1 = clock();
	for (int i=0; i< 10000000; i++)
		t1 = mktime(&tm1);
		//t2 = mktime1(2000, 1, 1, 0, 0, 0);
	pthread_t t1, t2, t3, t4;
	//pthread_create(&t1, NULL, test, NULL);
	//pthread_create(&t2, NULL, test, NULL);
	//pthread_create(&t3, NULL, test, NULL);
	//pthread_create(&t4, NULL, test, NULL);
	//pthread_join(t1, NULL);
	//pthread_join(t2, NULL);
	//pthread_join(t3, NULL);
	//pthread_join(t4, NULL);
	cout << clock()-c1 << endl;
}
