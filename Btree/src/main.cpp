//============================================================================
// Name        : main.cpp
// Author      : Ice
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================



#include "ConsoleLog.h"

#include "Btree.h"
#include <iostream>
#include <string>
#include <memory>
#include <ctime>

using namespace std;

int main() {
	clock_t t1 = clock();
	Btree tree("Debug/btree.data");
	tree.insert("a", 0);
	Log << "time: " << clock()-t1 << endl;
}
