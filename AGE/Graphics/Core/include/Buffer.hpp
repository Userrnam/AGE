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
    VkBuffer getBuffer() { return buffer; }

    void create(BufferCreateInfo& info);
    void destroy();
};

} // namespace age::core
