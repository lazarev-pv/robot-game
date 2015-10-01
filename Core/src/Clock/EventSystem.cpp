#include <ChibiEngine/Clock/EventSystem.h>
#include <ChibiEngine/Common/ContainerUtils.h>
#include <ChibiEngine/Log/LoggingSystem.h>

using namespace game;
using namespace std;

const static string LOGGER_NAME = "EventSystem";
EventHandle EventSystem::lastId = 0;

EventSystem::Event::Event(bool infinit, int repeatTimes, const std::function<void()>& action):
        id(++lastId),
        infinit(infinit),
        repeatTimes(repeatTimes),
        action(action),
        begin(0),
        period(0),
        marked2Deletion(false)
{}

EventSystem::Event::Event(Clock::Time begin, Clock::TimeDelta period, bool infinit, int repeatTimes, const std::function<void()>& action):
        id(++lastId),
        infinit(infinit),
        repeatTimes(repeatTimes),
        action(action),
        begin(begin),
        period(period),
        marked2Deletion(false)
{}

EventSystem::EventSystem() :
    logger(Game::getLoggingSystem()->createLogger(LOGGER_NAME)){
}

void EventSystem::doNextFrame(const std::function<void()> &action, int repeatsNum) {
    perFrame.push_back(Event(false,repeatsNum,action));
}

void EventSystem::doEveryFrame(const std::function<void()> &action) {
    perFrame.push_back(Event(true,1,action));
}

EventSystem::EventIter EventSystem::findInsertPosition(
                            std::list<Event>&  eventsList,
                            Clock::Time time,
                            EventSystem::EventIter beginPosition){

    EventSystem::EventIter iter = beginPosition;
    while(iter != eventsList.end() && iter->begin<time){
       ++iter;
   }
    return iter;
}

EventSystem::EventIter EventSystem::findInsertPosition(list<Event>& eventsList, Clock::Time time){
    return findInsertPosition(eventsList,time, eventsList.begin());
}

EventHandle EventSystem::doOnce(ClockCHandle timeLine, const std::function<void()> &action, Clock::Time begin_in_mills){
	return schedule(timeLine, action, timeLine->getTime() + begin_in_mills, 0, 1);
}

EventHandle EventSystem::schedule(ClockCHandle timeLine, const std::function<void()> &action, Clock::Time start, Clock::TimeDelta period, int repeatsNum) {
    auto& eventsList = timeEvents[timeLine];
    eventsList.insert(findInsertPosition(eventsList,start),Event(start,period,false,repeatsNum,action));
    logger.debug("New time event scheduled at %i", start);
    return lastId;
}

EventHandle EventSystem::scheduleInfiteRepeat(ClockCHandle timeLine, const std::function<void()> &action, Clock::Time start, Clock::TimeDelta period) {
    auto& eventsList = timeEvents[timeLine];
    eventsList.insert(findInsertPosition(eventsList,start),Event(start,period,true,1,action));
    logger.debug("New infinit time event scheduled at %i", start);
    return lastId;
}

void EventSystem::deleteEvent(ClockCHandle timeLine, EventHandle event){
	if(event==0){
        logger.trace("Null event can't be deleted");
		return;
    }

	// Существует ли вообще такой Event
	for(auto& it :  timeEvents[timeLine]){
		if(it.id==event){
			it.marked2Deletion = true;
            logger.trace("Event prepared 2 delete");
		}
	}
}

void EventSystem::processEvents() {

    list<Event>::iterator iter = perFrame.begin();
    while(iter!= perFrame.end()){
    	if(!iter->marked2Deletion){
    		iter->action();
    	}else{
    		iter->infinit = false;
    		iter->repeatTimes = 0;
    	}

        if(!iter->infinit)
            iter->repeatTimes--;

        if(iter->repeatTimes<=0)
            iter = perFrame.erase(iter);
        else
            ++iter;
    }

    for(auto& iterator : timeEvents){
        Clock::Time currentTime = iterator.first->getTime();
        auto& eventList = iterator.second;

        // Обрабатываем список событий в таймлайне
        auto currentEventIter = eventList.begin();
        while( currentEventIter != eventList.end() && currentEventIter->begin<=currentTime){
            //Исполняем действие
        	if(!currentEventIter->marked2Deletion){
        		currentEventIter->action();
			}else{
				currentEventIter->infinit = false;
				currentEventIter->repeatTimes = 0;
			}

            //Уменьшаем количество повторений
            if(!currentEventIter->infinit)
            	currentEventIter->repeatTimes--;

            if(currentEventIter->repeatTimes>0){ //event еще повториться - вставляем новый
                auto insertPos = findInsertPosition(eventList, currentEventIter->begin+currentEventIter->period);
                currentEventIter->begin += currentEventIter->period;
                eventList.insert(insertPos, move(*currentEventIter));
            }

            //Удаляем обработанный
            currentEventIter = eventList.erase(currentEventIter);
        }
    }

}

