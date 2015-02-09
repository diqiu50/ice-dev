/*
 * HttpResponse.cpp
 *
 *  Created on: 2014Äê7ÔÂ17ÈÕ
 *      Author: Ice
 */

#include "HttpResponse.h"

#include <string.h>
#include <iostream>
#include <memory>

using namespace std;

HttpResponse::HttpResponse(const std::shared_ptr<URL> &url):mUrl(url) {
	readHeader();
}

HttpResponse::~HttpResponse() {
}

void  HttpResponse::readHeader() {
	size_t total_read = 0;
	int begin = 0;
	size_t pos = 0;
	string key, value, status;
	mHeaderBuff.resize(4096);
	do {
		boost::asio::mutable_buffers_1 header =
			boost::asio::buffer(mHeaderBuff.data()+total_read, 4096-total_read);
		size_t bytes_to_read = boost::asio::read(mUrl->socket, header,
			boost::asio::transfer_at_least(1));
		size_t pos = total_read;
		total_read += bytes_to_read;
		for (; pos<total_read;) {
			if (mHeaderBuff[pos++] == '\n') {
				status = string(mHeaderBuff.data()+begin, pos - begin - 2);
				//cout << status << endl;
				begin = pos;
				break;
			}
		}
	}while(status.empty());

	do {
		for (; pos<total_read;) {
			if (mHeaderBuff[pos++] == ':') {
				key = string(mHeaderBuff.data()+begin, pos - begin -1);
				begin = pos;
				break;
			}
		}
		for (; pos<total_read;) {
			if (mHeaderBuff[pos++] == '\n') {
				value = string(mHeaderBuff.data()+begin+1, pos-begin-2);
				//cout << key << ": " << value << endl;
				begin = pos;
				mParams[key] = value;
				break;
			}
		}
		if (pos == total_read) {
			boost::asio::mutable_buffers_1 header =
				boost::asio::buffer(mHeaderBuff.data()+total_read, 4096-total_read);
			size_t bytes_to_read = boost::asio::read(mUrl->socket, header,
				boost::asio::transfer_at_least(1));
			total_read += bytes_to_read;
		}
	}while(mHeaderBuff[begin] !='\r' && mHeaderBuff[begin+1] !='\n');

	size_t header_size = pos+2;
	mReturnCode = atoi(status.data() + 9);

	if (total_read-header_size > 0) {
		mBodyBuff.insert(mBodyBuff.begin(), mHeaderBuff.begin()+header_size,
				mHeaderBuff.begin()+total_read);
	}
	mHeaderBuff.resize(header_size);
}

boost::asio::const_buffer HttpResponse::readBody() {
	boost::asio::const_buffer buffer;
	if (mReturnCode != 200) {
		return buffer;
	}
	auto itr = mParams.find("Content-Length");
	if (itr != mParams.end()) {
		int length = atoi(itr->second.data());
		return readLengthBody(length);
	}else {
		auto itr = mParams.find("Transfer-Encoding");
		if (itr != mParams.end()) {
			if (itr->second == "chunked")
				return readChunkedBody();
		}
	}
	return buffer;
}

boost::asio::const_buffer HttpResponse::readLengthBody(const int length) {
	size_t size = mBodyBuff.size();
	size_t leftsize = length - size;
	mBodyBuff.resize(length);

	if (leftsize > 0) {
		boost::asio::mutable_buffers_1 buff =
				boost::asio::buffer(mBodyBuff.data()+size, leftsize);
		boost::asio::read(mUrl->socket, buff);
	}
	cout << mBodyBuff.data();
	return boost::asio::buffer(mBodyBuff);
}

boost::asio::const_buffer HttpResponse::readChunkedBody() {
	size_t total_read = mBodyBuff.size();
	size_t pos = 0;
	int begin = 0;
	int read_size = 0;
	do {
		for (; pos<total_read;) {
			if (mBodyBuff[pos++]=='\n') {
				char * endptr;
				int chunk_size = strtol(mBodyBuff.data()+begin, &endptr, 16);
				read_size = chunk_size + 2 - (total_read - pos);
				begin = pos + chunk_size + 2;
				break;
			}
		}
		if (pos == total_read) {
			read_size = 1;
		}
		if (read_size > 0) {
			int new_size = (read_size==1?100:read_size) + mBodyBuff.size();
			mBodyBuff.resize(new_size);
			boost::asio::mutable_buffers_1 buff =
				boost::asio::buffer(mBodyBuff.data()+total_read, mBodyBuff.size()-total_read);
			size_t bytes_to_read = boost::asio::read(mUrl->socket,
					buff, boost::asio::transfer_at_least(read_size));
			total_read += bytes_to_read;
			pos = begin;
		}
	}while (read_size > 0 && read_size != 2);
	mBodyBuff.resize(total_read);
	//cout << mBodyBuff.data();
	return boost::asio::buffer(mBodyBuff);
}

