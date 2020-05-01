#pragma once

#include "External/FastDelegate.h"
#include <memory>
#include <chrono>
#include <inttypes.h>
#include <time.h>

namespace age::events {

    class IEventData;

    typedef uint32_t EventType;
    typedef std::shared_ptr<IEventData> IEventDataPtr;
    typedef fastdelegate::FastDelegate1<IEventDataPtr> EventListenerDelegate;

    class IEventData {
    public:
        virtual const EventType& getEventType() const = 0;
        virtual float getTimeStamp() const = 0;
        virtual IEventDataPtr copy() const = 0;
        virtual const char* getName() const = 0;
    };

    class BaseEventData : public IEventData {
        const float timeStamp;

    public:
        explicit BaseEventData(const float timeStamp_ = 0.0f) : timeStamp(timeStamp_) {}
        virtual ~BaseEventData() {}
        virtual const EventType& getEventType() const = 0;
        float getTimeStamp() const { return timeStamp; }
    };

    class IEventManager {
    public:
		enum eConstants { INFINTE = 0xffffffff };

		explicit IEventManager(const char* pName, bool setAsGlobal);
		virtual ~IEventManager();

		// Registers a delegate function that will get called when the event type is
		// triggered. Returns true if successful, false if not.
		virtual bool addListener(const EventListenerDelegate& eventDelegate, const EventType& type) = 0;

		// Removes a delegate / event type pairing from the internal tables.
		// Returns false if the pairing was not found.
		virtual bool removeListener (const EventListenerDelegate& eventDelegate, const EventType& type) = 0;

		// Fires off event NOW. This bypasses the queue entirely and immediately
		// calls all delegate functions registered for the event.
		virtual bool triggerEvent(const IEventDataPtr& pEvent) const = 0;

		// Fires off event. This uses the queue and will call the delegate function
		// on the next call to VTickVUpdate(), assuming there’s enough time.
		virtual bool queueEvent(const IEventDataPtr& pEvent) = 0;

		// Finds the next-available instance of the named event type and remove it
		// from the processing queue. This may be done up to the point that it is
		// actively being processed...e.g.: is safe to happen during event
		// processing itself.
		//
		// If allOfType is true, then all events of that type are cleared from the
		// input queue.
		//
		// returns true if the event was found and removed, false otherwise
		virtual bool abortEvent(const EventType& type, bool allOfType = false) = 0;

		// Allows for processing of any queued messages, optionally specify a
		// processing time limit so that the event processing does not take too
		// long. Note the danger of using this artificial limiter is that all
		// messages may not in fact get processed.
		//
		// returns true if all messages ready for processing were completed, false
		// otherwise (e.g. timeout).
		virtual bool update(uint32_t maxMillis = INFINTE) = 0;
    
		// Getter for the main global event manager. This is the event manager that
		// is used by the majority of the engine, though you are free to define your
		// own as long as you instantiate it with setAsGlobal set to false.
		// It is not valid to have more than one global event manager.
		static IEventManager* get();
	};

} // naemspace age::events