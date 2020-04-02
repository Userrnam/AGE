#pragma once

#include <vulkan/vulkan.h>

namespace core {

VkInstance getInstance();
void debugEnable(bool);
void createInstance(const char *appName, uint32_t appVersion);
void destroy();

} // namespace core
