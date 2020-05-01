#pragma once

#include "EventManager.hpp"
#include <list>
#include <map>

const uint32_t EVENTMANAGER_NUM_QUEUES = 2;

namespace age::events {

	class EventManager : public IEventManager {
		typedef std::list<EventListenerDelegate> EventListenerList;
		typedef std::map<EventType, EventListenerList> EventListenerMap;
		typedef std::list<IEventDataPtr> EventQueue;

        EventListenerMap eventListeners;
		EventQueue queues[EVENTMANAGER_NUM_QUEUES];
		uint32_t activeQueue;

	public:
		explicit EventManager(const char* pName, bool setAsGlobal);
		virtual ~EventManager() {}

		virtual bool addListener(const EventListenerDelegate& eventDelegate, const EventType& type);

		virtual bool removeListener(const EventListenerDelegate& eventDelegate, const EventType& type);

		virtual bool triggerEvent(const IEventDataPtr& pEvent) const;
		virtual bool queueEvent(const IEventDataPtr& pEvent);
		virtual bool abortEvent(const EventType& type, bool allOfType = false);

		virtual bool update(uint32_t maxMillis = INFINTE);
	};

} // namespace age::events