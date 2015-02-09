/*
 * main.cpp
 *
 *  Created on: 2013-5-8
 *      Author: Ice
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <time.h>
#include <atomic>

using namespace std;

mutex m_lock;

void threadFun(int flag) {
	cout << this_thread::get_id() << endl;
	int i=1000000000;
	int sum=0;
	//atomic<int> cnt = ATOMIC_VAR_INIT(0);
	clock_t t1 = clock();
	while(i--) {
		if (flag == 1){
			lock_guard<mutex> lock(m_lock);
			sum += i;
		}else if (flag == 2){
			sum += i;
		}
		else {
			//atomic_fetch_add(&cnt, i);
			__sync_fetch_and_add(&sum, i);
		}
	}
	cout << "cost time: " << clock()-t1 << endl;
}

int main(int argc, char** argv) {
	thread t1(threadFun, 1);
	t1.join();
	thread t2(threadFun, 2);
	t2.join();
	thread t3(threadFun, 3);
	t3.join();
	return 0;
}




