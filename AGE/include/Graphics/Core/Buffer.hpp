#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <cstring>

#include "Image.hpp"

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
    void destroy();

    VkBuffer getBuffer() const { return std::move(m_buffer); }
    VkDeviceMemory getMemory() const { return std::move(m_memory); }

    void create(BufferCreateInfo& info);

    void copyTo(Buffer& buffer, VkDeviceSize size, VkDeviceSize srcOffset=0, VkDeviceSize dstOffset=0);
    void copyTo(Image& image, VkDeviceSize srcOffset=0);
    void loadData(const void* data, VkDeviceSize size, VkDeviceSize offset=0);
};

Buffer createDeviceLocalBuffer(void* data, VkDeviceSize size, VkBufferUsageFlags usage);

} // namespace age::core
