#include <string.h>

#include "Event.hpp"
#include "../Containers/Arena.hpp"


namespace age {

void Event::setStructure(void* p, size_t size) {
    m_structure = Arena::allocate(size);
    memcpy(m_structure, p, size);
}

} // namespace age
