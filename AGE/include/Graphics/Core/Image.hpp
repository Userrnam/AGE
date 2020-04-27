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
    VkImage image = VK_NULL_HANDLE;
    VkImageView imageView = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
public:
    VkImage getImage() { return image; }
    VkImageView getView() { return imageView; }

    void create(ImageCreateInfo& info);
    void destroy();
};

} // namespace age::core
