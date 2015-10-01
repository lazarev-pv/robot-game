
#ifndef __CLOCKSYSTEM_H_
#define __CLOCKSYSTEM_H_

#include <ChibiEngine/Clock/Clock.h>
#include <vector>
#include <ChibiEngine/Log/Logger.h>
#include <ChibiEngine/Game.h>
#include <memory>

namespace game {

	class Logger;
	class ClockSystem : private UniqueClass {
	public:
		ClockSystem();

		void updateGlobalTime(Clock::TimeDelta delta);
		ClockHandle createClock(Clock::Time initialTime = 0);
		inline const Logger& getLogger();

		/** Таймер независимый от искажения других таймеров (ui, течении времени в игре и т.п.)
		*  Имеет смысл использовать для тех таймлайнов, течение которых не планируется искажать или останавливать,
		*  например таймлайн пересчета координат камеры для рендеринга */
		ClockCHandle getImmutableClock() const;
	private:
		std::vector<std::unique_ptr<Clock>> clocks;
		ClockCHandle immutbleClock;
		Logger logger;
	};

	inline const Logger& ClockSystem::getLogger(){
		return logger;
	}

	inline ClockCHandle ClockSystem::getImmutableClock() const{
		return immutbleClock;
	}
}


#endif
