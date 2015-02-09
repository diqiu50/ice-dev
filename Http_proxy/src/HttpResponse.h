/*
 * HttpResponse.h
 *
 *  Created on: 2014Äê7ÔÂ17ÈÕ
 *      Author: Ice
 */

#ifndef HTTPRESPONSE_H_
#define HTTPRESPONSE_H_

#include "URL.h"

#include <boost/asio.hpp>
#include <memory>
#include <vector>

class HttpResponse {
	int mReturnCode;
	std::vector<char> mHeaderBuff;
	std::map<std::string, std::string> mParams;
	std::vector<char> mBodyBuff;
	std::shared_ptr<URL> mUrl;

public:
	HttpResponse(const std::shared_ptr<URL> &url);
	~HttpResponse();
	boost::asio::const_buffer readBody();

private:
	void readHeader();
	boost::asio::const_buffer readLengthBody(const int length);
	boost::asio::const_buffer readChunkedBody();
};

#endif /* HTTPRESPONSE_H_ */
