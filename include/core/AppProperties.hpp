#pragma once

#include <vulkan/vulkan.h>

namespace core {

struct AppProperties {
    bool debugEnable;
    VkInstance instance;

    void createInstance(const char *appName, uint32_t appVersion);
};

} // namespace core

extern core::AppProperties appProperties;
