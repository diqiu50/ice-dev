#include <iostream>
#include <vector>
#include <boost/coroutine/all.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;

class Call;


queue<Call*> req_queue2;
mutex m2;
condition_variable cv2;

class Call
{
	typedef boost::coroutines::symmetric_coroutine<void> coro;
	coro::call_type mCoroutine;
	int mId;

	public:

	Call(int id):
	mCoroutine(boost::bind(&Call::func, this, _1)),
	mId(id)
	{
	}

	void run(){
		mCoroutine();
	}

	virtual void func(coro::yield_type &yield)
	{
			//do something
			cout << mId << ": do step 1" << endl;

			// 5. make an asynchronous network request
			unique_lock<mutex> lk(m2);
			req_queue2.push(this);
			cv2.notify_one();
			lk.unlock();

			// 6. give up the cup and suspend current coroutine.
			yield();

			cout << mId << ": do step 2" << endl;
		
	}
};

void
make_coroutine_call(int &a)
{
	Call *ab = new Call(a);
	ab->run();	
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
					Call *req = req_queue2.front();		
					req_queue2.pop();
					lk.unlock();
					sleep(rand()%5+1);
					// 8. resume the coroutine execution. 
					req->run();	
					// 9 releae the coroutine.
					delete req;
				}
			}
		}
	);

	//1. add some call request.
	unique_lock<mutex> lk(m);
	req_queue.push(1);
	req_queue.push(2);
	req_queue.push(3);
	req_queue.push(4);
	req_queue.push(5);
	req_queue.push(6);
	req_queue.push(7);
	req_queue.push(8);
	cv.notify_one();
	lk.unlock();
	
	t1.join();
	return 0;
}


//g++ coroutine.cpp -std=c++11 -lboost_system -lboost_coroutine

