//
// sync_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "HttpRequest.h"

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <memory>

#include <stdio.h>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

using namespace std;
using namespace boost::asio;

int main(int argc, char* argv[]) {
	vector<int> v;
	//shared_ptr<URL> url = make_shared<URL>("www.baidu.com");
	//HttpRequest req(url, HttpRequest::HTTP_GET);
	//shared_ptr<HttpResponse> resp = req.getResponse();
	//resp->readBody();
	//return 0
	ssl::context ctx(ssl::context::sslv23);
	//ctx.set_default_verify_paths();
	//openssl x509 -inform der -in a.cer -out a.pem
	//ctx.add_verify_path("D:\\Users\\Ice\\Documents\\Visual Studio 2012\\Git\\ice-dev\\Http_proxy\\Debug\\certs\\");
	ctx.load_verify_file("D:\\Users\\Ice\\Documents\\Visual Studio 2012\\Git\\ice-dev\\Http_proxy\\Debug\\certs\\CA.pem");
	io_service ioservice;
	ssl::stream<ip::tcp::socket> sock(ioservice, ctx);
	//ip::tcp::endpoint ep(ip::address_v4::from_string("190.30.252.131"), (short)443);
	//sock.lowest_layer().connect(ep);
	ip::tcp::resolver resolver(ioservice);
	ip::tcp::resolver::query query("github.com", "https");
	boost::asio::connect(sock.lowest_layer(), resolver.resolve(query));

	sock.set_verify_mode(ssl::verify_peer);
	sock.set_verify_callback(ssl::rfc2818_verification("github.com"));
	/*
	sock.set_verify_callback([](bool preverified, ssl::verify_context &ctx) -> bool {
	    char subject_name[256];
	    X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
	    X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
	    std::cout << "Verifying " << subject_name << "\n";
		return false;
	});
	*/

	sock.handshake(ssl::stream_base::client);
	boost::asio::streambuf request;
	ostream req(&request);
	req << "GET / HTTP/1.0\r\n";
	req << "Host: github.com\r\n";
	req << "Accept: */*\r\n";
	req << "Accept-Encoding: deflate\r\n";
	req << "Connection: close\r\n\r\n";
	size_t size = boost::asio::write(sock, request);
	boost::asio::streambuf response;
	size = boost::asio::read(sock, response, boost::asio::transfer_at_least(4096));
	cout << size << endl;
	cout << string(buffers_begin(response.data()),
			buffers_end(response.data())) << endl;
	return 0;
}
