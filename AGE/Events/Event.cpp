#include <string.h>

#include "Event.hpp"
#include "../Containers/Arena.hpp"


namespace age {

Arena eventArena;

void Event::setStructure(void* p, size_t size) {
    m_structure = eventArena.allocate(size);
    memcpy(m_structure, p, size);
}

} // namespace age
