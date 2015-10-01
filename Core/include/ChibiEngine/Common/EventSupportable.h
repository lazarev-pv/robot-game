
#ifndef __EVENT_SUPPORTABLE_H_
#define __EVENT_SUPPORTABLE_H_

#include <string>
#include <unordered_map>
#include <algorithm>
#include <functional>

namespace game{

	class EventSupportable {
	public:
		using EventListener = std::function<void(const std::string&)>;
		using EventListenerHandle = unsigned;

		static const std::string MOVE_EVENT;

		virtual ~EventSupportable() = default;
		virtual EventListenerHandle addEventListener(const EventListener& listener) = 0;
		virtual void removeEventListener(EventListenerHandle listener) = 0;
	};

	class EventSupportableImpl : public EventSupportable {
	public:

		inline EventListenerHandle addEventListener(const EventListener& listener) override;
		inline void removeEventListener(EventListenerHandle listener) override;

		inline void setSilentMode(bool silent);
		inline bool isSilentMode();
		inline void fireEvent(const std::string& event);

	private:
		bool silentMode = false;
		std::unordered_map<EventListenerHandle, EventListener> listeners;
		EventListenerHandle lastId = 0;

	};

	inline EventSupportable::EventListenerHandle EventSupportableImpl::addEventListener(const EventSupportable::EventListener& listener){
		lastId++;
		listeners.insert({lastId,listener});
		return lastId;
	}

	inline void EventSupportableImpl::removeEventListener(EventSupportable::EventListenerHandle listener){
		listeners.erase(listener);
	}

	inline void EventSupportableImpl::setSilentMode(bool silent){
		silentMode = silent;
	}

	inline bool EventSupportableImpl::isSilentMode(){
		return silentMode;
	}

	inline void EventSupportableImpl::fireEvent(const std::string& event){
		if(silentMode)
			return;
		for(auto& listener : listeners){
			listener.second(event);
		}
	}
}



#endif
