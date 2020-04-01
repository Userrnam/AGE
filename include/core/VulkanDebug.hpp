#pragma once

#include <vector>
#include <vulkan/vulkan.h>

namespace core {

bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers);
void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

} // namespace core
