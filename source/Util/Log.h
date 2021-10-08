#pragma once
#include <string>
#include <mutex>

enum LogType{
		ERROR,WARNING,INFO,DEBUG
	};
class Log
{
	static std::recursive_mutex mutex;
	static void (*fun)(LogType e,std::string data);
	static bool isDebug;
public:
	Log() = delete;
	
	
	static void error(std::string);
	static void warning(std::string);
	static void info(std::string);
	static void debug(std::string);	

	static void setErrorCallback(void (*fun)(LogType e,std::string));
	static bool isDebugMode();
};

