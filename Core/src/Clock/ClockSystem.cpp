#include <ChibiEngine/Clock/ClockSystem.h>
#include <ChibiEngine/Log/LoggingSystem.h>

using namespace game;
using namespace std;

static const string LOGGER_NAME = "ClockLog";

ClockSystem::ClockSystem()
    :logger(Game::getLoggingSystem()->createLogger(LOGGER_NAME)){
    immutbleClock = createClock(0);
}

void ClockSystem::updateGlobalTime(Clock::TimeDelta delta) {
    for(auto& clock : clocks){
        clock->updateTime(delta);
    }
}

ClockHandle ClockSystem::createClock(Clock::Time initialTime) {
    ClockHandle clock = new Clock(initialTime);
    clocks.push_back(unique_ptr<Clock>(clock));
    return clock;
}

