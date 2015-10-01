#ifndef __CHIBIENGINE_CLOCK_CLOCK_HANDLES_
#define __CHIBIENGINE_CLOCK_CLOCK_HANDLES_

namespace game{

    class Clock;
    using ClockHandle = Clock*;
    using ClockCHandle = const Clock*;

    class EventSystem;
    using EventHandle = int;
    constexpr EventHandle NullEventHandle = 0;

}

#endif