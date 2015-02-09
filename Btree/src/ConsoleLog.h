#ifndef Log_ConsoleLog
#define Log_ConsoleLog

#include <iostream>
#include <sstream>
#include <windows.h>
#include <assert.h>

#define Log \
	ConsoleLog::log(); \
{	std::ostringstream ss; \
	ss << "[LOG]"
#define Error \
	ConsoleLog::error(); \
{	std::ostringstream ss; \
	ss << "[Error]"
#define Warn \
	ConsoleLog::warn(); \
{	std::ostringstream ss; \
	ss << "[Warn]"
#define Debug \
{	std::ostringstream ss;\
	ss << "<0x" << std::hex << GetCurrentThreadId() << std::dec << "::" << __FUNCTION__ << ":" << __LINE__ << ">"
#define endl ""; std::cout << ss.str() << std::endl;}

#define Assert(x) assert(x);

class ConsoleLog
{
public:
	static void log()
	{
		
	}
	static void error()
	{
		
	}
	static void warn()
	{
		
	}
	static void debug()
	{
		
	}
};

#endif
