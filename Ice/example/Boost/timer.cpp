#include <iostream>
#include <boost/asio/high_resolution_timer.hpp>
#include <chrono>
#include <thread>
#include <vector>


using namespace boost::asio;
using namespace std;

int count = 0;

struct xxx{
	int i;
	xxx(int ii) {
		i = ii;
	}
	void operator()(const boost::system::error_code& e)
	{
		if (e != boost::asio::error::operation_aborted)
		{
			// Timer was not cancelled, take necessary action.
		}
		count++;
		cout << i <<"," << time(NULL) << endl;
	}
};

int main(int argc, char** argv) {

	boost::asio::io_service io;

	high_resolution_timer tx(io);
	tx.expires_from_now(std::chrono::seconds(60));
	xxx xx(0);
	tx.async_wait(xx);

	//vector<high_resolution_timer> tts;
	for (int i = 0 ; i < 10000000; i++)
	{
		high_resolution_timer t(io);
		t.expires_from_now(std::chrono::milliseconds(1000));
		xxx x(i);
		t.async_wait(x);
		//tts.push_back(t);
	}

	std::thread th([&]() {
				io.run();
				});

	high_resolution_timer  t1(io);
	t1.expires_from_now(std::chrono::seconds(2));
	xxx x1(2);
	t1.async_wait(x1);


	sleep(3);

	high_resolution_timer  t2(io);
	t2.expires_from_now(std::chrono::seconds(2));
	xxx x2(3);
	t2.async_wait(x2);

	th.join();
	cout << "count:" << count << endl;
	return 0;
}
 
//g++ main.cpp -std=c++11 -lboost_system -pthread
