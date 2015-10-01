#include <ChibiEngine/Clock/Clock.h>

using namespace game;

Clock::TimeDelta Clock::getTimeDelta() const{
    return currentDelta;
}

Clock::Time Clock::getTime() const{
    return currentTime;
}

bool Clock::isPaused() const{
	return paused;
}

void Clock::scaleClock(float scaleFactor) {
    this->scaleFactor = scaleFactor;
}

void Clock::pause(bool isPaused) {
    this->paused = isPaused;
}

void Clock::reset(){
    paused = false;
    currentTime = 0.0;
    currentDelta =0.0f;
    scaleFactor = 1.0000;
}

Clock::Clock(Time time){
    reset();
    currentTime = time;
}

void Clock::updateTime(TimeDelta delta) {
    currentDelta = !paused ? delta*scaleFactor : 0.0000;
    currentTime += currentDelta;
}

