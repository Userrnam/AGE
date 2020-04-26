#pragma once

#include <vulkan/vulkan.h>

namespace age::core {

struct BufferCreateInfo {
    VkDeviceSize size;
    VkBufferUsageFlags usage;
    VkMemoryPropertyFlags memoryProperties;
};

class Buffer {
    VkBuffer buffer;
    VkDeviceMemory memory;
public:
    ~Buffer();
    void create(BufferCreateInfo& info);
};

} // namespace age::core
