#pragma once

#include <vulkan/vulkan.h>

namespace core {

struct AppProperties {
    bool debugEnable;
    VkDebugUtilsMessengerEXT debugMessenger;

    VkInstance instance;

    void createInstance(const char *appName, uint32_t appVersion);
    void destroy();
};

} // namespace core

extern core::AppProperties appProperties;
