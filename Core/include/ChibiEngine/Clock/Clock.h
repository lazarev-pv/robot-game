
#ifndef __CLOCK_H_
#define __CLOCK_H_

#include <ChibiEngine/Common/ClassUtils.h>
#include <ChibiEngine/Clock/ClockHandles.h>

namespace game{

    class Clock : private UniqueClass {
    public:
        using Time = long;
        using TimeDelta = long;

        Clock(Time time);

        void updateTime(TimeDelta delta);
        void scaleClock(float scaleFactor);
        void pause(bool isPaused);
        void reset();

        bool isPaused() const;
        TimeDelta getTimeDelta() const;
        Time getTime() const;
        inline float getScaleFactor() const;

    private:

        Time currentTime;
        TimeDelta currentDelta;
        float scaleFactor;
        bool paused;
    };

    inline float Clock::getScaleFactor() const{
        return scaleFactor;
    }

}
#endif
