#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <cstring>

#include "Core/Image.hpp"

namespace age {

class BufferCreateInfo {
    VkDeviceSize m_size;
    VkBufferUsageFlags m_usage;
    VkMemoryPropertyFlags m_memoryProperties;

    friend class Buffer;
public:
    inline BufferCreateInfo& setSize(VkDeviceSize size) {
        m_size = size;
        return *this;
    }

    inline BufferCreateInfo& setUsage(VkBufferUsageFlags usage) {
        m_usage = usage;
        return *this;
    }

    inline BufferCreateInfo& setMemoryProperties(VkMemoryPropertyFlags properties) {
        m_memoryProperties = properties;
        return *this;
    }
};

class VertexBufferCreateInfo {
    BufferCreateInfo createInfo;
public:
    VertexBufferCreateInfo() {
        createInfo.setMemoryProperties(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        createInfo.setUsage(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
    }

    inline VertexBufferCreateInfo& setSize(VkDeviceSize size) {
        createInfo.setSize(size);
        return *this;
    }

    inline operator BufferCreateInfo() {
        return createInfo;
    }
};

class IndexBufferCreateInfo {
    BufferCreateInfo createInfo;
public:
    IndexBufferCreateInfo() {
        createInfo.setMemoryProperties(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        createInfo.setUsage(VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
    }

    inline IndexBufferCreateInfo& setSize(VkDeviceSize size) {
        createInfo.setSize(size);
        return *this;
    }

    inline operator BufferCreateInfo() {
        return createInfo;
    }
};

class UniformBufferCreateInfo {
    BufferCreateInfo createInfo;
public:
    UniformBufferCreateInfo() {
        createInfo.setMemoryProperties(VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        createInfo.setUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
    }

    inline UniformBufferCreateInfo& setSize(VkDeviceSize size) {
        createInfo.setSize(size);
        return *this;
    }

    operator BufferCreateInfo() {
        return createInfo;
    }
};

// all information, except VkBuffer is not used in drawing and used only for creating something
// maybe make some structure, that will hold this data in dynamic memory?
// than move some extra information to this structure, so creating
// descriptor sets for example is much easier
class Buffer {
protected:
    VkBuffer m_buffer = VK_NULL_HANDLE;
    VkDeviceMemory m_memory = VK_NULL_HANDLE;
    VkDeviceSize m_size = VK_NULL_HANDLE;
public:
    void destroy();

    VkBuffer getBuffer() const { return m_buffer; }
    VkDeviceMemory getMemory() const { return m_memory; }
    VkDeviceSize getSize() const { return m_size; }

    void create(const BufferCreateInfo& info);

    void copyTo(Buffer& buffer, VkDeviceSize size, VkDeviceSize srcOffset=0, VkDeviceSize dstOffset=0);
    void copyTo(core::Image& image, VkDeviceSize srcOffset=0);
    void load(const void* data, VkDeviceSize size, VkDeviceSize offset=0);
    void loadDeviceLocal(const void* data, VkDeviceSize size, VkDeviceSize offset=0);
};

} // namespace age
