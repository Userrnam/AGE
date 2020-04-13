#pragma once

#include <vector>

namespace core {

bool checkDeviceExtensionSupport(const VkPhysicalDevice device, const std::vector<const char*>& deviceExtensions);
void getRequeredExtensions(std::vector<const char*>& extensions, bool enableValidationLayers);

// returns number supported features
// returns -1 if device cannot be used
int deviceSupportedFeatures(const VkPhysicalDevice device);

VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevel);
VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
VkFormat findDepthFormat();
void createImage(uint32_t width, uint32_t height, uint32_t mipLevel, 
    VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, 
    VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, 
    VkDeviceMemory& imageMemory);
uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
VkSampleCountFlagBits getMaxSampleCount();

} // namespace core
