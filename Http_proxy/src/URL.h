/*
 * URL.h
 *
 *  Created on: 2014Äê8ÔÂ24ÈÕ
 *      Author: Ice
 */

#ifndef URL_H_
#define URL_H_

#include <string>
#include <boost/asio.hpp>

class URL {
private:
	boost::asio::io_service io_service;

public:
	std::string protocol;
	std::string host;
	std::string port;
	std::string path;
	boost::asio::ip::tcp::socket socket;

public:
	URL(const std::string &url);
	~URL();
};

#endif /* URL_H_ */
