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
		enum { max_length = 1500, max_cache_size=10000, timer_frq=5};
		udp::socket socket_;
		char* data_;
		udp::endpoint send_endpoint_;
		boost::asio::high_resolution_timer timer_;

		struct packet {
			int packetid;
			char data[max_length];
			bool issend;
		};

		packet *packets;
		packet *bengin;
		int begin;
		int end;
		int crt_send_idx;
		int last_noack_idx;

		int send_count;
		int recv_count;
		int window_size;
		int window_dist;
		int packetid;

	public:
		server(boost::asio::io_service& io_service, udp::endpoint &ep)
			: socket_(io_service, udp::endpoint(udp::v4(), 0)), 
				send_endpoint_(ep),
				data_(new char[max_length]),
				timer_(io_service),
				packets(new packet[max_cache_size]),
				begin(0),
				end(max_cache_size+1),
				crt_send_idx(0),
				last_noack_idx(0),
				recv_count(0),
				window_size(1),
				packetid(1)
		{
			timer_.expires_from_now(chrono::milliseconds(1));
			timer_.async_wait(boost::bind(&server::sendmsg_timer_handler, this, boost::asio::placeholders::error));;
			do_receive();

		}

		void sendmsg_timer_handler(boost::system::error_code e) {
			window_size = get_window_size();
			window_dist = 2*window_size;
			cout << "window size: " << window_size << endl;
			do_send(window_size);
			timer_.expires_from_now(chrono::milliseconds(timer_frq));
			timer_.async_wait(boost::bind(&server::sendmsg_timer_handler, this, boost::asio::placeholders::error));;
		}


		void do_receive()
		{
			socket_.async_receive_from(
				boost::asio::buffer(data_, max_length), send_endpoint_,
				[this](boost::system::error_code ec, std::size_t bytes_recvd)
				{
					assert(bytes_recvd == max_length);
					if (data_[0] == 1) {
						int num = (max_length-1)/4;
						recv_count += num;
						int *data = (int*)(data_+1);
						ostringstream ss;
						ss << "recv ack:";
						while(num>0 && *data !=0) {
							int ack_pktid = *data;
							int dist = ack_pktid - packets[begin].packetid;
							int idx = move(begin, dist);
							if (idx == last_noack_idx) {
								last_noack_idx=move(idx,1);
								while((packets[last_noack_idx].issend == true) && last_noack_idx != crt_send_idx) {
									last_noack_idx=move(last_noack_idx,1);
								}
							}

							packets[idx].issend = true;
							ss << " " << ack_pktid;
							num--;
							data++;
						}
						cout << ss.str() << endl;
						recv_count -= num;
						begin = move(last_noack_idx, -1);
						end = move(begin, max_cache_size-1);
					}
					do_receive();
				});
		}

		int get_window_size() {
			double miss_rate = (double)recv_count/window_size;
			double size = window_size*2;
			if (miss_rate<0.98)
				size = window_size*1.5;
			if (miss_rate<0.90)
				size = window_size*1.2;
			if (miss_rate<0.80)
				size = window_size*1;
			if (miss_rate<0.60)
				size = window_size*0.8;
			if (miss_rate<0.40)
				size = window_size*0.6;
			if (miss_rate<0.20)
				size = window_size*0.2;
			int newsize;
			if (size < 1)
				newsize = 1;
			else if(size>100)
				newsize = 100;
			else
				newsize = (int)size;
			recv_count = 0;
			return  newsize;
		}

		void do_send(int size)
		{
			//cout << "crtidx: " << crt_send_idx << " last_noack_idx: " << last_noack_idx << endl;
			char *data;
			int idx = last_noack_idx;
			if (distance(crt_send_idx, last_noack_idx) >= window_dist) {
				//cout << "dist " << distance(crt_send_idx, last_noack_idx) << " w_dist " << window_dist << endl;
				int endidx = move(crt_send_idx, -window_dist);
				//cout << "packet idx: " << idx << endl;
				//cout << "packet endidx: " << endidx << endl;
				while(size>0 && idx != endidx) {
					if (packets[idx].issend == false) {
						data = packets[idx].data;
						//cout << "resend packet idx: " << idx << endl;
						//cout << "resend packet endidx: " << endidx << endl;
						cout << "resend packet: " << packets[idx].packetid << endl;
						socket_.async_send_to(
							boost::asio::buffer(data, max_length), send_endpoint_,
							[data, this](boost::system::error_code ec, std::size_t bytes_send)
							{
								assert(bytes_send == max_length);
								//do_receive();
							});
						size--;
					} 
					idx = move(idx, 1);
				}
			}

			while(size > 0) {
				packets[crt_send_idx].packetid = packetid;
				data = packets[crt_send_idx].data;
				data[0] = 0;
				*(int*)(data+1) = packetid;
				packets[crt_send_idx].issend = false;
				cout << "send packet: " << packetid << endl;
				socket_.async_send_to(
					boost::asio::buffer(data, max_length), send_endpoint_,
					[data, this](boost::system::error_code ec, std::size_t bytes_send)
					{
						assert(bytes_send == max_length);
						//do_receive();
					});
				crt_send_idx = move(crt_send_idx, 1);
				packetid++;
				size--;
			}
		}

		int move(int crtidx, int len) {
			int idx = crtidx+len;
			if (idx>=max_cache_size)
				idx = idx - max_cache_size;
			if (idx < 0)
				idx = max_cache_size + idx;
			assert(idx>=0 && idx <max_cache_size);
			return idx;
		}

		int distance(int end, int from) {
			int dist;
			if (end>=from){
				dist = end-from;	
			} else {
				dist = end + (max_cache_size-from);
			}
			return dist;
		}

};

int main(int argc, char* argv[])
{
	try
	{
		boost::asio::io_service io_service;
		int port = 8877;
		udp::endpoint ep(address_v4::from_string("192.168.99.184"), port);
		server s(io_service, ep);

		/*
		thread t1([port, &s](){
				udp::endpoint ep(address_v4::from_string("192.168.99.183"), port);
				while (1) {
					sleep(1);
				}
			});
		*/

		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}

//g++ -std=c++11 -lboost_thread -lboost_system
