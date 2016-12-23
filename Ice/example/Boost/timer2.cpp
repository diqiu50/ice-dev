#include <cstdlib>
#include <vector>
#include <iostream>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/asio/high_resolution_timer.hpp>
#include <thread>

using namespace std;

using boost::asio::ip::udp;
using boost::asio::ip::address_v4;



boost::asio::io_service io_service;
boost::asio::high_resolution_timer timer_(io_service);
void timeout(boost::system::error_code e)
{
	//timer_.cancel();
	cout << "xxxxxxxxxxxx" << e << endl;
}
void timeout1(boost::system::error_code e)
{
	//timer_.cancel();
	cout << "xxxxxxxxxxxx1" << e << endl;
}
void timeout2(boost::system::error_code e)
{
	//timer_.cancel();
	cout << "xxxxxxxxxxxx2" << e << endl;
}


int main() {
	timer_.expires_from_now(chrono::milliseconds(1000));
	timer_.async_wait(boost::bind(&timeout, boost::asio::placeholders::error));

	timer_.expires_from_now(chrono::milliseconds(8000));
	timer_.async_wait(boost::bind(&timeout1, boost::asio::placeholders::error));

	timer_.expires_from_now(chrono::milliseconds(5000));
	timer_.async_wait(boost::bind(&timeout2, boost::asio::placeholders::error));
	int i=0;
	//i = timer_.cancel();
	//int i = timer_.cancel_one();
	cout << "c: " << i << endl;

	//timer_.async_wait(boost::bind(&timeout1, boost::asio::placeholders::error));
	thread t([&](){
		io_service.run();
		});
	sleep(10);
}

//g++ -std=c++11 main.cpp  -I /home2/ice/Zykie.AOS/Include/ -lboost_system -pthread
