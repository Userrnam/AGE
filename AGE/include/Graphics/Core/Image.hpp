#pragma once

#include "vulkan/vulkan.h"

namespace age::core {

struct ImageCreateInfo {
    VkExtent2D extent;
    uint32_t mipLevel;
    VkSampleCountFlagBits numberOfSamples;
    VkFormat format;
    VkImageTiling tiling;
    VkImageUsageFlags imageUsage;
    VkMemoryPropertyFlags memoryProperties;
    VkImageAspectFlags aspectFlags;
};

class Image {
    VkImage m_image = VK_NULL_HANDLE;
    VkImageView m_imageView = VK_NULL_HANDLE;
    VkDeviceMemory m_memory = VK_NULL_HANDLE;
    VkFormat m_format;
    VkExtent2D m_extent;
    uint32_t m_mipLevel;
public:
    VkImage getImage() const { return m_image; }
    VkImageView getView() const  { return m_imageView; }
    uint32_t getWidth() const { return m_extent.width; }
    uint32_t getHeight() const { return m_extent.height; }
    uint32_t getMipLevel() const { return m_mipLevel; }

    void create(ImageCreateInfo& info);
    void destroy();

    void generateMipmaps();
};

} // namespace age::core
