/*
 * HttpRequest.cpp
 *
 *  Created on: 2014Äê7ÔÂ16ÈÕ
 *      Author: Ice
 */

#include "HttpRequest.h"


#include <boost/asio.hpp>
#include <array>
#include <iostream>

using namespace std;
using boost::asio::ip::tcp;


HttpRequest::HttpRequest(const shared_ptr<URL> &url, Method method): mUrl(url), mMethod(method) {
}

HttpRequest::~HttpRequest() {
}

shared_ptr<HttpResponse>  HttpRequest::getResponse() {
	//cout << "=====================================================" << endl;
	boost::asio::streambuf request;
	std::ostream request_stream(&request);
	request_stream << getMethodStr() << " " << mUrl->path << " HTTP/1.1\r\n";
	request_stream << "Host: " << mUrl->host << "\r\n";
	request_stream << "Accept: */*\r\n";
	request_stream << "Connection: Keep-Alive\r\n\r\n";
	//cout << &request << endl;
	boost::asio::write(mUrl->socket, request);
	//cout << "---------------Http Header------------------------" << endl;
	return make_shared<HttpResponse>(mUrl);
}
