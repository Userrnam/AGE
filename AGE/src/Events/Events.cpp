#include "Events.hpp"

// To define a new event please generate 32-bit UUID.
// The reason using this and not enums is to not recompile an entire application 
// every time there is a new event added.

// Example:
// const EventType EvtData_NewActor::eventType(0xa3814acd)