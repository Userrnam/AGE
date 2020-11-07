#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace age::core {

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

// FIXME: add prefered formats
VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

} // namespace age::core
