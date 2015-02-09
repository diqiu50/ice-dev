/*
 * HttpRequest.h
 *
 *  Created on: 2014Äê7ÔÂ16ÈÕ
 *      Author: Ice
 */

#ifndef HTTPREQUEST_H_
#define HTTPREQUEST_H_

#include "HttpResponse.h"
#include "URL.h"

#include <string>
#include <memory>

class HttpRequest {
public:
	enum Method {
		HTTP_POST,
		HTTP_GET
	};

private:
	std::shared_ptr<URL> mUrl;
	Method mMethod;

public:
	HttpRequest(const std::shared_ptr<URL> &url, Method method);
	~HttpRequest();

	std::shared_ptr<HttpResponse> getResponse();

private:
	std::string getMethodStr() { return mMethod==HTTP_GET?"GET":"POST";}
};

#endif /* HTTPREQUEST_H_ */
