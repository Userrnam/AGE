#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <cstring>

namespace age::core {

struct BufferCreateInfo {
    VkDeviceSize size;
    VkBufferUsageFlags usage;
    VkMemoryPropertyFlags memoryProperties;
};

class Buffer {
protected:
    VkBuffer m_buffer = VK_NULL_HANDLE;
    VkDeviceMemory m_memory = VK_NULL_HANDLE;
public:
    VkBuffer getBuffer() { return m_buffer; }

    void create(BufferCreateInfo& info);
    void copyTo(Buffer& buffer, VkDeviceSize size, VkDeviceSize srcOffset=0, VkDeviceSize dstOffset=0);
    void loadData(void* data, VkDeviceSize size);
    void destroy();
};

Buffer createDeviceLocalBuffer(void* data, VkDeviceSize size, VkBufferUsageFlags usage);

} // namespace age::core
