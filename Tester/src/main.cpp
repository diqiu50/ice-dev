/*
 * main.cpp
 *
 *  Created on: 2014Äê9ÔÂ24ÈÕ
 *      Author: Ice
 */


#include <memory>
#include <thread>
#include <iostream>

using namespace std;

void fun() {
	cout << 1111 << endl;
}

int main(int argc, char ** argv) {
	shared_ptr<char> c(new char[500]);
	c.get();
	thread t(fun);
	t.join();
}
