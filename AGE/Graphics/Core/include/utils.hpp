#pragma once

#include <vector>

namespace age::core {

bool checkDeviceExtensionSupport(const VkPhysicalDevice device, const std::vector<const char*>& deviceExtensions);
void getRequeredExtensions(std::vector<const char*>& extensions, bool enableValidationLayers);

// returns number supported features
// returns -1 if device cannot be used
int deviceSupportedFeatures(const VkPhysicalDevice device);
VkSampleCountFlagBits getMaxSampleCount();

VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
VkFormat findDepthFormat();

VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevel);

} // namespace age::core
