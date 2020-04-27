#pragma once

#include <vulkan/vulkan.h>

namespace age::core {

struct BufferCreateInfo {
    VkDeviceSize size;
    VkBufferUsageFlags usage;
    VkMemoryPropertyFlags memoryProperties;
};

class Buffer {
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
public:
    VkBuffer getBuffer() { return buffer; }

    void create(BufferCreateInfo& info);
    void destroy();
};

} // namespace age::core
