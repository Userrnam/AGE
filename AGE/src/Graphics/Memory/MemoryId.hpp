#pragma once

#include <vulkan/vulkan.h>

namespace age::core {

typedef uint32_t AllocAddress;

struct MemoryId {
    AllocAddress address;
    VkDeviceMemory memory;
    VkBuffer buffer;
};

} // namespace age::core
