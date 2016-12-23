//
// async_udp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <vector>
#include <iostream>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/asio/high_resolution_timer.hpp>

using boost::asio::ip::udp;
using boost::thread;
using boost::asio::ip::address_v4;
using namespace std;

class server
{
	private:

		enum { max_length = 1500, timer_frq=5};
		udp::socket socket_;
		char* data_;
		vector<int> packet_ids;
		udp::endpoint sender_endpoint_;
		boost::asio::high_resolution_timer timer_;

	public:
		server(boost::asio::io_service& io_service, short port)
			: socket_(io_service, udp::endpoint(udp::v4(), port)), 
				data_(new char[max_length]),
				timer_(io_service)
		{
			timer_.expires_from_now(chrono::milliseconds(1));
			timer_.async_wait(boost::bind(&server::sendack_timer_handler, this, boost::asio::placeholders::error));;
			do_receive();

		}

		void sendack_timer_handler(boost::system::error_code e) {
			if (!packet_ids.empty())
				do_sendack();
			timer_.expires_from_now(chrono::milliseconds(timer_frq));
			timer_.async_wait(boost::bind(&server::sendack_timer_handler, this, boost::asio::placeholders::error));;
		}

		void do_sendack() {
			int num = (max_length-1)/4;

			char *buff = new char[max_length];
			memset(buff, 0, max_length);
			buff[0] = 1;
			int *data = (int*)(buff+1);
			int idx = 0;
			for (int i=0;i<packet_ids.size(); i++) {
				if (idx>num)
				{
					do_send(buff);
					buff = new char[max_length];
					memset(buff, 0, max_length);
					buff[0] = 1;
					data = (int*)(buff+1);
					idx = 0;
				}
				data[idx] = packet_ids[i];
				idx++;
			}

			do_send(buff);
			packet_ids.clear();
		}

		void do_receive()
		{
			socket_.async_receive_from(
					boost::asio::buffer(data_, max_length), sender_endpoint_,
					[this](boost::system::error_code ec, std::size_t bytes_recvd)
					{
						assert(bytes_recvd == max_length);
						if (data_[0] == 0) {
							int packetid = *(int*)(data_+1);
							cout << "recv packet: " << packetid << endl;
							packet_ids.push_back(packetid);
						}
						do_receive();
					});
		}

		void do_send(char *buff)
		{
			int num = (max_length-1)/4;
			int *data = (int*)(buff+1);
			ostringstream ss;
			ss << "send ack:";
			while(num>0 && *data !=0) {
				ss << " " << *data;
				num--;
				data++;
			}
			cout << ss.str() << endl;
			socket_.async_send_to(
					boost::asio::buffer(buff, max_length), sender_endpoint_,
					[buff, this](boost::system::error_code ec, std::size_t bytes_send)
					{
						assert(bytes_send == max_length);
						delete []buff;
						//do_receive();
					});
		}

};

int main(int argc, char* argv[])
{
	try
	{
		boost::asio::io_service io_service;
		int port = 8877;
		server s(io_service, port);
		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}

//g++ -std=c++11 -lboost_thread -lboost_system
