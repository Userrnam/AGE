#include <vulkan/vulkan.h>

#include "Index.hpp"

namespace age {

uint32_t Index16::getIndexType() {
    return VK_INDEX_TYPE_UINT16;
}

uint32_t Index32::getIndexType() {
    return VK_INDEX_TYPE_UINT32;
}

} // namespace age
