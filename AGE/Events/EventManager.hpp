#pragma once

#include <vector>
#include <stdint.h>

#include "Event.hpp"

#define EVENT_CALLBACK(class, method) \
	age::detail::Handler<class> __ ## class ## method{ \
    typeid(age::detail::GetHandler<decltype(&class::method)>::eventType).hash_code(), \
    this, \
    age::detail::forceCast<age::detail::EventHandler>(&class::method), \
    age::detail::GetHandler<decltype(&class::method)>::success}

namespace age {

namespace detail {

typedef void(*EventHandler)(void*, void*);

class Event {
    size_t m_eventId;
    void *m_structure;

    void setStructure(void* p, size_t size);
public:
    void* getStructurePointer() {
        return m_structure;
    }

    template<typename T>
    void setStructure(T& s) {
        setStructure((void*)&s, sizeof(T));
        m_eventId = typeid(T).hash_code();
    }

    template<typename T>
    const T& getStructure() const {
        return *((T*)m_structure);
    }

    operator size_t() const {
        return m_eventId;
    }
};

// stores methods and pointers to thier's classes
struct CallbackStructure {
	detail::EventHandler eventHandler;
	void* caller;
};

} // namespace detail

class EventManager {
    static std::unordered_map<size_t, std::vector<detail::CallbackStructure>> m_callbacks;
    static std::vector<detail::Event> m_events;
public:
    // called by age::Application
    static void __init();
    static void __destroy();
    static void __processEvents();

    static void __sendEvent(const detail::Event& e);

    template<typename T>
    static void sendEvent(const T& val) {
        detail::Event e;
        e.setStructure(val);
        __sendEvent(e);
    }

    // called by Handler
    static void __registerCallback(size_t eid, void* caller, detail::EventHandler);
    static void __forgetCallback(void* caller, size_t eid);
};

namespace detail {

template<typename T>
class Handler {
	size_t m_eid;
	void* m_p;

public:
	Handler(uint64_t eid, T* p, EventHandler e, std::true_type) {
		m_eid = eid;
		m_p = p;
		EventManager::__registerCallback(eid, p, e);
	}

	~Handler() {
		EventManager::__forgetCallback(m_p, m_eid);
	}

	Handler(const Handler&) = delete;
};

template<typename Target, typename Current>
inline Target forceCast(Current c) {
	union Hack { Target t; Current c; };
	Hack hack;
	hack.c = c;
	return hack.t;
}

template<typename T>
struct GetHandler {
	static constexpr std::false_type success;
	static constexpr std::false_type eventType;
};

template<typename T, typename U>
struct GetHandler<void(T::*)(const U&)> {
	static constexpr std::true_type success;
	static constexpr U eventType;
};

} // namespace detail

} // namespace age

namespace age {

typedef uint32_t KeyCode;

bool isKeyPressed(KeyCode keyCode);
Vector2f getCursorPosition();

} // namespace age
