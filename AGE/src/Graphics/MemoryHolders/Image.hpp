#pragma once

#include <vulkan/vulkan.h>

#include "../Memory/DeviceAlloc.hpp"

namespace age::core {

// Images are always device local

class ImageCreateInfo {
    VkExtent2D m_extent;
    VkSampleCountFlagBits m_sampleCount;
    VkFormat m_format;
    VkImageUsageFlags m_imageUsage;
    VkImageAspectFlags m_aspectFlags;

    friend class Image;
public:
    inline ImageCreateInfo& setExtent(const VkExtent2D& extent) {
        m_extent = extent;
        return *this;
    }

    inline ImageCreateInfo& setSampleCount(VkSampleCountFlagBits count) {
        m_sampleCount = count;
        return *this;
    }

    inline ImageCreateInfo& setFormat(VkFormat format) {
        m_format = format;
        return *this;
    }

    inline ImageCreateInfo& setImageUsage(VkImageUsageFlags usage) {
        m_imageUsage = usage;
        return *this;
    }

    inline ImageCreateInfo& setAspectFlags(VkImageAspectFlags aspectFlags) {
        m_aspectFlags = aspectFlags;
        return *this;
    }
};

class Image {
    VkImage m_image = VK_NULL_HANDLE;
    VkImageView m_imageView = VK_NULL_HANDLE;
    VkDeviceMemory m_memory = VK_NULL_HANDLE;
    VkFormat m_format;
    VkExtent2D m_extent;
public:
    VkImage getImage() const { return m_image; }
    VkImageView getView() const  { return m_imageView; }
    uint32_t getWidth() const { return m_extent.width; }
    uint32_t getHeight() const { return m_extent.height; }

    void create(ImageCreateInfo& info);
    void destroy();
};

} // namespace age::core
