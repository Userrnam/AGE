#include <vector>

#include "QueueFamilyIndicies.hpp"
#include "Core/Core.hpp"

namespace age::core {

std::vector<VkQueueFamilyProperties> getQueueFamilyProperties(VkPhysicalDevice device) {
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamiles(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamiles.data());

	return queueFamiles;
}

// FIXME: works only if queue supports both graphics family and present family
// reason: framebuffer
QueueFamilyIndicies findQueueFamilies(VkPhysicalDevice device) {
	QueueFamilyIndicies indicies;
	uint32_t queueFamilyCount = 0;
	
	auto queueFamiles = getQueueFamilyProperties(device);
	
	int i = 0;
	for (const auto& queueFamily : queueFamiles) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, apiCore.window.surface, &presentSupport);
			if (presentSupport) {
				indicies.graphicsFamily = i;
				indicies.presentFamily = i;
			}
		}
		i++;
	}
	return indicies;
}

} // namespace age::core
