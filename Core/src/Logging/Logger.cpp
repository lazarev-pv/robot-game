#include <ChibiEngine/Log/LoggingSystem.h>

#include <iostream>

#ifdef MY_ANDROID
#include <android/log.h>
static const char* TAG = "AndTower";
#endif

using namespace game;
using namespace std;


static const int FG_BLACK    = 30;
static const int FG_RED      = 31;
static const int FG_GREEN    = 32;
static const int FG_YELLOW   = 33;
static const int FG_BLUE     = 34;
static const int FG_MAGENTA  = 35;
static const int FG_CYAN     = 36;
static const int FG_DEFAULT  = 39;
static const int BG_RED      = 41;
static const int BG_GREEN    = 42;
static const int BG_BLUE     = 44;
static const int BG_DEFAULT  = 49;


#define ENUM_TO_STRING(A,B)\
case A:						\
	str<<#B;				\
	break;

inline void print(Logger::LogLevel level, std::ostream& str){
	switch(level){
        ENUM_TO_STRING(Logger::NOT_SPECIFIED,NOT_SPECIFIED)
		ENUM_TO_STRING(Logger::NONE,NONE)
		ENUM_TO_STRING(Logger::ERROR,ERROR)
		ENUM_TO_STRING(Logger::WARN,WARN)
		ENUM_TO_STRING(Logger::INFO,INFO)
		ENUM_TO_STRING(Logger::DEBUG,DEBUG)
		ENUM_TO_STRING(Logger::TRACE,TRACE)
	}
}

#undef ENUM_TO_STRING


#ifdef MY_ANDROID
int convertPrio(Logger::LogLevel level){
	switch(level){
	case Logger::ERROR:
		return ANDROID_LOG_ERROR;
	case Logger::WARN:
		return ANDROID_LOG_WARN;
	case Logger::INFO:
		return ANDROID_LOG_INFO;
	case Logger::DEBUG:
		return ANDROID_LOG_DEBUG;
	case Logger::TRACE:
		return ANDROID_LOG_VERBOSE;
	default:
		return ANDROID_LOG_UNKNOWN;
	}
}
#endif

void Logger::log(Logger::LogLevel level, const std::string &message, int color)  const{
#ifndef MY_ANDROID
	if(this->level==Logger::NOT_SPECIFIED || this->level < level)
        return;

    cout <<"\033["<<color<<"m";
    //cout << "\033[1;31mbold red text\033[0m\n";
    std::cout<<this->name<<' ';
    print(level, std::cout);
    std::cout<<' '<<message;
    cout <<"\033["<<FG_DEFAULT<<"m";
    cout<<std::endl;
#else
    __android_log_write(convertPrio(level), TAG, message.c_str());
#endif
}

void Logger::log(Logger::LogLevel level, int color, const char *format, va_list args)  const{
#ifndef MY_ANDROID
	if(this->level==Logger::NOT_SPECIFIED || this->level < level)
      return;

    cout <<"\033["<<color<<"m";
    //cout << "\033[1;31mbold red text\033[0m\n";
    cout<<this->name<<' ';
    print(level, std::cout);
    cout<<' ';
    vprintf(format, args);
    cout <<"\033["<<FG_DEFAULT<<"m";
    cout<<std::endl;
#else
    __android_log_vprint(convertPrio(level), TAG, format, args);
#endif
}

Logger::Logger(Logger::LogLevel level, const std::string& name)
    :level(level),name(name){
}

void Logger::trace(const std::string &message) const{
    log(Logger::TRACE, message, FG_MAGENTA);
}

void Logger::debug(const std::string &message) const{
    log(Logger::DEBUG, message, FG_BLUE);
}

void Logger::info(const std::string &message) const{
    log(Logger::INFO, message, FG_DEFAULT);
}

void Logger::warn(const std::string &message) const{
    log(Logger::WARN, message, FG_YELLOW);
}

void Logger::error(const std::string &message) const{
    log(Logger::ERROR, message, FG_RED);
}

void Logger::trace(const char *format, ...) const{
    va_list args;
	va_start(args, format);
    log(Logger::TRACE, FG_MAGENTA, format, args);
    va_end(args);
}

void Logger::debug(const char *format, ...) const{
    va_list args;
	va_start(args, format);
    log(Logger::DEBUG, FG_BLUE, format, args);
    va_end(args);
}
void Logger::info(const char *format, ...) const{
    va_list args;
	va_start(args, format);
    log(Logger::INFO, FG_DEFAULT, format, args);
    va_end(args);
}
void Logger::warn(const char *format, ...) const{
    va_list args;
	va_start(args, format);
    log(Logger::WARN, FG_YELLOW, format, args);
    va_end(args);
}
void Logger::error(const char *format, ...) const{
    va_list args;
	va_start(args, format);
    log(Logger::ERROR, FG_RED, format, args);
    va_end(args);
}

