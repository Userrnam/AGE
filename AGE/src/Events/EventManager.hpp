#pragma once

#include <vector>

#include "Event.hpp"

namespace age::EventManager {

void init();
void destroy();

void sendEvent(Event& event);
const std::vector<Event>& getEvents();
void clearEvents();

} // namespace age
