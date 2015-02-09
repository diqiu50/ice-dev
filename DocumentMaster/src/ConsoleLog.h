#ifndef Log_ConsoleLog
#define Log_ConsoleLog

#include <iostream>
#include <sstream>
#include <windows.h>
#include <assert.h>


#define Info \
do{	std::ostream& out = ConsoleLog::info(); \
	std::ostringstream __ss; \
	__ss << "[INFO]"

#define Log \
do{	std::ostream& out = ConsoleLog::log(); \
	std::ostringstream __ss; \
	__ss << "[LOG]"
#define Error \
do{	std::ostream& out = ConsoleLog::error(); \
	std::ostringstream __ss; \
	__ss << "[Error]"
#define Warn \
do{	std::ostream& out = ConsoleLog::warn(); \
	std::ostringstream __ss; \
	__ss << "[Warn]"
#define Debug \
do{	std::ostream& out = ConsoleLog::debug(); \
	std::ostringstream __ss;\
	__ss << "<0x" << std::hex << GetCurrentThreadId() << std::dec << "::" << strstr(__FILE__, "src")+4 << ":" << __LINE__ << ">"
#define endl ""; out << __ss.str() << std::endl;}while(0)

//#define Assert(x) ConsoleLog::assert1((x)!=0)
#define Assert(x) assert(x)

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
	static void assert1(int r) {
		if (r == 0) {
			assert(r);
		} else {
			assert(r);
		}
	}
};

#endif
