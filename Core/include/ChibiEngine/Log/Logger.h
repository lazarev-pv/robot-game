#ifndef __LOGGING_H
#define	__LOGGING_H

#include <string>
#include <map>
#include <cstdarg>

namespace game{

    class Logger{
    public:
    	enum LogLevel{NOT_SPECIFIED = -2,NONE = -1, ERROR = 0, WARN = 1, INFO = 2, DEBUG = 3, TRACE = 4};

    	Logger(LogLevel level, const std::string& name);

        void trace(const std::string &message) const;
		void debug(const std::string &message) const;
        void info(const std::string &message) const;
        void warn(const std::string &message) const;
        void error(const std::string &message) const;
        
        void trace(const char *format, ...) const;
        void debug(const char *format, ...) const;
        void info(const char *format, ...) const;
        void warn(const char *format, ...) const;
        void error(const char *format, ...) const;
        
    private:    
        void log(LogLevel level, const std::string &message, int color) const;
        void log(LogLevel level, int color, const char *format, std::va_list args) const;
        
        LogLevel level;
        std::string name;
    };
    
}

#endif

