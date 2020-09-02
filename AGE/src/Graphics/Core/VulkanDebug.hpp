#pragma once

#include <vector>
#include <vulkan/vulkan.h>

namespace age::core {

bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers);
void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
void setupDebugMessenger(VkInstance instance, VkDebugUtilsMessengerEXT *debugMessenger);
void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator);

} // namespace age::core
