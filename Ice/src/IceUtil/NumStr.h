#include <iostream>
#include <sstream>

#ifndef IceUtil_NumStr 
#define IceUtil_NumStr

template<typename T>
std::string ntos(T num)
{
	std::ostringstream ss;
	ss << num;
	return ss.str();
}

template<typename T>
T ston(const std::string &str)
{
	std::istringstream ss(str);
	T num;
	return ss >> num ? num : 0;
}

#endif
