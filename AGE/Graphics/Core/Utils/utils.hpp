#pragma once

#include <vector>
#include <vulkan/vulkan.h>

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

VkCommandBuffer beginSingleTimeCommands();
void endSingleTimeCommands(VkCommandBuffer commandBuffer);

VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

} // namespace age::core
