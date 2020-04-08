#pragma once

#include <vector>

namespace core {

struct DeviceRequirements {
    std::vector<const char*> deviceExtensions;

    DeviceRequirements() {
        deviceExtensions = {
            "VK_KHR_swapchain"
        };
    }
};

} // namespace core
