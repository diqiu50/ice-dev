/*
 * URL.cpp
 *
 *  Created on: 2014Äê8ÔÂ24ÈÕ
 *      Author: Ice
 */

#include "URL.h"

#include <iostream>
#include <memory>

using namespace std;
using boost::asio::ip::tcp;

URL::URL(const string &url):protocol("http"), port("80"), path("/"), socket(io_service) {
	// TODO Auto-generated constructor stub
	cout << url << endl;
	size_t begin, end ;
	begin = end = 0;
	begin = url.find("://", begin);
	if (begin != string::npos) {
		if (url[0]=='h' && url[1]=='t' && url[2]=='t' && url[3]=='p') {
			begin = 7;
		}else {
			protocol = url.substr(0, begin);
			begin += 3;
		}
	} else {
		begin = 0;
	}
	end = url.find(':', begin);
	if (end != string::npos) {
		host = url.substr(begin, end-begin);
		begin = end+1;
		end = url.find('/', begin);
		if (end != string::npos) {
			port = url.substr(begin, end-begin);
			path = url.substr(end, url.length()-end);
		}else {
			port = url.substr(begin, url.length()-end);
		}
	}else {
		end = url.find('/', begin);
		if (end != string::npos) {
			host = url.substr(begin, end-begin);
			path = url.substr(end, url.length()-end);
		}else
		{
			host = url.substr(begin, url.length()-begin);
		}
	}
	//cout << protocol << "," << host << "," << port << "," << path << endl;
    tcp::resolver resolver(io_service);
    tcp::resolver::query query(host, port);
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

    //tcp::endpoint ep = *endpoint_iterator;
    //std::cout << ep << endl;

    boost::asio::connect(socket, endpoint_iterator);
}

URL::~URL() {
}

