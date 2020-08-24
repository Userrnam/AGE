#pragma once

#include <vulkan/vulkan.h>

namespace age::core::deviceAlloc {

typedef uint32_t AllocAddress;

struct MemoryId {
    AllocAddress address;
    VkDeviceMemory memory;
    VkBuffer buffer;
};

MemoryId allocBuffer(uint32_t size, VkBufferUsageFlags usage);

void freeBuffer(MemoryId memory);

void destroy();

} // namespace age::core::deviceAlloc
