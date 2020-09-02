#pragma once

#include <optional>
#include <vulkan/vulkan.h>

namespace age::core {

struct QueueFamilyIndicies {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

std::vector<VkQueueFamilyProperties> getQueueFamilyProperties(VkPhysicalDevice device);
QueueFamilyIndicies findQueueFamilies(VkPhysicalDevice device);

} // namespace age::core
