#include "EventManagerImpl.hpp"

namespace age::events {

    EventManager::EventManager(const char* pName, bool setAsGlobal) : IEventManager(pName, setAsGlobal) {
        activeQueue = 0;
    }

    bool EventManager::addListener(const EventListenerDelegate& eventDelegate, const EventType& type) {
        EventListenerList& eventListenerList = eventListeners[type]; // Finds or creates an entry for given event type

        for (auto it = eventListenerList.begin(); it != eventListenerList.end(); ++it) {
            if (eventDelegate == (*it)) {
                // TO DO(si-7): Implement some kind of warning system
                return false;
            }
        } 

        eventListenerList.push_back(eventDelegate);
        return true;
    }

    bool EventManager::removeListener(const EventListenerDelegate& eventDelegate, const EventType& type) {
        bool success = false;

        auto findIt = eventListeners.find(type);
        if (findIt != eventListeners.end()) {
            EventListenerList& listeners = findIt->second;
            for (auto it = listeners.begin(); it != listeners.end(); ++it) {
                if (eventDelegate == (*it)) {
                    listeners.erase(it);
                    success = true;

                    break;
                }
            }
        }

        return success;
    }

    bool EventManager::triggerEvent(const IEventDataPtr& pEvent) const {
        bool processed = false;

        auto findIt = eventListeners.find(pEvent->getEventType());
        if (findIt != eventListeners.end()) {
            const EventListenerList& eventListenerList = findIt->second;
            for (EventListenerList::const_iterator it = eventListenerList.begin(); it != eventListenerList.end(); ++it) {
                EventListenerDelegate listener = (*it);
                listener(pEvent);
                processed = true;
            }
        }

        return processed;
    }

    bool EventManager::queueEvent(const IEventDataPtr& pEvent) {
        // TODO(si-7): Check for correct activeQueue index
        
        auto findIt = eventListeners.find(pEvent->getEventType());
        if (findIt != eventListeners.end()) {
            queues[activeQueue].push_back(pEvent);
            return true;
        }
        
        return false;
    }

    bool EventManager::abortEvent(const EventType& type, bool allOfType) {
        // TODO(si-7): Check for correct activeQueue index

        bool success = false;
        EventListenerMap::iterator findIt = eventListeners.find(type);

        if (findIt != eventListeners.end()) {
            EventQueue& eventQueue = queues[activeQueue];
            auto it = eventQueue.begin();
            while (it != eventQueue.end()) {
                auto thisIt = it;
                ++it;

                if ((*thisIt)->getEventType() == type) {
                    eventQueue.erase(thisIt);
                    success = true;
                    if (!allOfType) {
                        break;
                    }
                }
            }
        }

        return success;
    }

    bool EventManager::update(uint32_t maxMillis) {
        uint32_t currMs = clock()*1000/CLOCKS_PER_SEC; // TODO(si-7): clock() is process-based, would be more reliable to find something OS based
        uint32_t maxMs = ((maxMillis == IEventManager::INFINTE) ? (IEventManager::INFINTE) : (currMs+maxMillis));

        int queueToProcess = activeQueue;
        activeQueue = (activeQueue + 1) % EVENTMANAGER_NUM_QUEUES;
        queues[activeQueue].clear();

        while (!queues[queueToProcess].empty()) {
            IEventDataPtr pEvent = queues[queueToProcess].front();
            queues[queueToProcess].pop_front();

            const EventType& eventType = pEvent->getEventType();

            auto findIt = eventListeners.find(eventType);
            if (findIt != eventListeners.end()) {
                const EventListenerList& eventListeners = findIt->second;

                for (auto it = eventListeners.begin(); it != eventListeners.end(); ++it) {
                    EventListenerDelegate listener = (*it);
                    listener(pEvent);
                }
            }

            currMs = clock()*1000/CLOCKS_PER_SEC;
            if (maxMillis != IEventManager::INFINTE && currMs >= maxMs) {
                // TODO(si-7): Implement logging system to log the timeout and event processing abort
                break;
            }
        }

        bool queueFlushed = (queues[queueToProcess].empty());
        if (!queueFlushed) {
            while (!queues[queueToProcess].empty()) {
                IEventDataPtr pEvent = queues[queueToProcess].back();
                queues[queueToProcess].pop_back();
                queues[activeQueue].push_front(pEvent);
            }
        }

        return queueFlushed;
    }

} // namespace age::events