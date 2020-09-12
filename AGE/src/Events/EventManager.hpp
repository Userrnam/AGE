#pragma once

#include <vector>
#include <stdint.h>

#include "Event.hpp"

namespace age::EventManager {

void init();
void destroy();

void sendEvent(Event& event);
const std::vector<Event>& getEvents();
void clearEvents();

} // namespace age

namespace age {

typedef uint32_t KeyCode;

bool isKeyPressed(KeyCode keyCode);

} // namespace age
