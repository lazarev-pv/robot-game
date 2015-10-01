#ifndef __EVENT_SYSTEM_HEADER_
#define __EVENT_SYSTEM_HEADER_

#include <ChibiEngine/Clock/Clock.h>
#include <ChibiEngine/Game.h>
#include <functional>
#include <list>
#include <map>
#include <ChibiEngine/Log/Logger.h>

namespace game{

	class EventSystem : private UniqueClass {
    public:
        EventSystem();

        void doNextFrame(const std::function<void()> &action, int repeatsNum = 1);
        void doEveryFrame(const std::function<void()> &action);
        EventHandle schedule(ClockCHandle timeLine, const std::function<void()> &action, Clock::Time start, Clock::TimeDelta period, int repeatsNum);
        EventHandle doOnce(ClockCHandle timeLine, const std::function<void()> &action, Clock::Time begin_in_mills);
        EventHandle scheduleInfiteRepeat(ClockCHandle timeLine, const std::function<void()> &action, Clock::Time start, Clock::TimeDelta period);

        void processEvents();
        void deleteEvent(ClockCHandle timeLine, EventHandle event);

    private:
        struct Event{
            Event(bool infinit, int repeatTimes, const std::function<void()>& action);
            Event(Clock::Time begin, Clock::TimeDelta period, bool infinit, int repeatTimes, const std::function<void()>& action);

            Event(const Event& event) = delete;
            Event& operator=(const Event& other) = delete;
            Event(Event&& event) = default;
            Event& operator=(Event&& other) = default;

            EventHandle id;
            bool infinit;
            int repeatTimes;
            std::function<void()> action;
            Clock::Time begin;
            Clock::TimeDelta period;
            bool marked2Deletion;
        };
        static EventHandle lastId;

        typedef std::list<Event>::iterator EventIter;
        typedef std::list<Event>::const_iterator CEventIter;
        EventIter findInsertPosition(std::list<Event>&  eventsList, Clock::Time time, EventIter beginPosition);
        EventIter findInsertPosition(std::list<Event>&  eventsList, Clock::Time time);

        std::list<Event> perFrame;
        Logger logger;
        std::map<ClockCHandle, std::list<Event>> timeEvents;
    };
}

#endif
