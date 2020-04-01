#pragma once

#include <vulkan/vulkan.h>

namespace core {

class AppProperties {
    bool m_debugEnable;
    VkDebugUtilsMessengerEXT m_debugMessenger;
    VkInstance m_instance;
public:
    void debugEnable(bool);
    void createInstance(const char *appName, uint32_t appVersion);
    void destroy();
};

} // namespace core
