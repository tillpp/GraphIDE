#include "Log.h"
#include <iostream>

void standartErrorCallback(LogType e, std::string msg)
{
	if (e == LogType::DEBUG ? Log::isDebugMode() : 1)
		std::cout << "[LOG]" << msg << std::endl;
}
std::recursive_mutex Log::mutex;
void (*Log::fun)(LogType e, std::string data) = &standartErrorCallback;
bool Log::isDebug = false;

void Log::error(std::string msg){
	mutex.lock();
	fun(LogType::ERROR,msg);
	mutex.unlock();
}
void Log::warning(std::string msg){
	mutex.lock();
	fun(LogType::WARNING,msg);
	mutex.unlock();
}
void Log::info(std::string msg){
	mutex.lock();
	fun(LogType::INFO,msg);
	mutex.unlock();
}
void Log::debug(std::string msg){
	mutex.lock();
	fun(LogType::DEBUG,msg);
	mutex.unlock();
}

void Log::setErrorCallback(void (*fun)(LogType,std::string)){
	mutex.lock();
	Log::fun = fun;
	mutex.unlock();
}
bool Log::isDebugMode()
{
	std::lock_guard<std::recursive_mutex> lock(mutex);
	return isDebug;
}