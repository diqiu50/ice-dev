#ifndef Log_ConsoleLog
#define Log_ConsoleLog

#include <iostream>
#include <sstream>
#include <windows.h>
#include <assert.h>


#define Info \
do{	ostream& out = ConsoleLog::info(); \
	std::ostringstream ss; \
	ss << "[INFO]"

#define Log \
do{	ostream& out = ConsoleLog::log(); \
	std::ostringstream ss; \
	ss << "[LOG]"
#define Error \
do{	ostream& out = ConsoleLog::error(); \
	std::ostringstream ss; \
	ss << "[Error]"
#define Warn \
do{	ostream& out = ConsoleLog::warn(); \
	std::ostringstream ss; \
	ss << "[Warn]"
#define Debug \
do{	ostream& out = ConsoleLog::debug(); \
	std::ostringstream ss;\
	ss << "<0x" << std::hex << GetCurrentThreadId() << std::dec << "::" << strrchr(__FILE__, '\\')+1 << ":" << __LINE__ << ">"
#define endl ""; out << ss.str() << std::endl;}while(0)

#define Assert(x) assert(x);

class ConsoleLog {
public:
	static std::ostream& info() {
		return std::cout;
	}
	static std::ostream& log() {
		return std::cout;
	}
	static std::ostream& error() {
		return std::cout;
	}
	static std::ostream& warn() {
		return std::cout;
	}
	static std::ostream& debug() {
		return std::cout;
	}
};

#endif
