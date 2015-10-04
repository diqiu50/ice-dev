#include <iostream>
#include <vector>
#include <boost/coroutine/all.hpp>
#include <boost/asio.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;


queue<boost::coroutines::symmetric_coroutine<int>::call_type *> req_queue2;
mutex m2;
condition_variable cv2;

void
make_coroutine_call(int &a)
{
	// 3. create a coroutine
    boost::coroutines::symmetric_coroutine<int>::call_type *coro_a = new 
		boost::coroutines::symmetric_coroutine<int>::call_type(
        [&](boost::coroutines::symmetric_coroutine<int>::yield_type& yield){
			//do something
			int i = yield.get();
			cout << i << ": do step 1" << endl;
			a++;

			// 5. make an asynchronous network request
			unique_lock<mutex> lk(m2);
			req_queue2.push(coro_a);
			cv2.notify_one();
			lk.unlock();

			// 6. give up the cup and suspend current coroutine.
			yield();
			cout << i << ": do step 2" << endl;
        }
	);

	// 4. start the coroutine
	(*coro_a)(a);
}

queue<int> req_queue;
mutex m;
condition_variable cv;



int main(int argc, char **argv) {
	// this thread sumilate the jimocall request proc thread
	thread t1(
		[](){
			while(1) {
				unique_lock<mutex> lk(m);
				if (req_queue.empty()){
					cv.wait(lk);	
					lk.unlock();
				}
				else {
					int req = req_queue.front();		
					req_queue.pop();
					lk.unlock();
					// 2. proc request usning coroutine.
					make_coroutine_call(req);
				}
			}
		}
	);

	
	// this thread sumilate the call back thread of recive data form network
	thread t2(
		[](){
			while(1) {
				unique_lock<mutex> lk(m2);
				if (req_queue2.empty()){
					cv2.wait(lk);	
					lk.unlock();
				}
				else {
					// 7. the data has complete. wake up the coroutine.
					boost::coroutines::symmetric_coroutine<int>::call_type *req = req_queue2.front();		
					req_queue2.pop();
					lk.unlock();
					sleep(rand()%5);
					// 8. resume the coroutine execution. 
					(*req)(5);
					// 9 releae the coroutine.
					delete req;
				}
			}
		}
	);

	//1. add some call request.
	unique_lock<mutex> lk(m);
	req_queue.push(3);
	req_queue.push(4);
	req_queue.push(5);
	req_queue.push(6);
	cv.notify_one();
	lk.unlock();
	
	t1.join();

}



