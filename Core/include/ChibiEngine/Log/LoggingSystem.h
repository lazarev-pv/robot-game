#ifndef __LOGGINGSYSTEM_H_
#define __LOGGINGSYSTEM_H_

#include <ChibiEngine/Log/Logger.h>
#include <ChibiEngine/Game.h>

namespace game{

	class LoggingSystem : private UniqueClass {
	public:
		LoggingSystem();
		Logger createLogger(const std::string &name);
	private:
		std::map<std::string,Logger::LogLevel> loggersLevel;
		std::map<std::string,Logger::LogLevel> logLevelStringPresentations;
	};

}

#endif
