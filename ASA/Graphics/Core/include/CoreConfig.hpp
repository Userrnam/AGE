#pragma once

#include <vector>

namespace core {

struct CoreConfig {
    std::vector<const char*> deviceExtensions = { "VK_KHR_swapchain" };
    struct {
        bool geometryShader = false;
        bool tesselationShader = false;
        bool samplerAnistropy = false;
        bool sampleRateShading = false;
    } features;
    
// FIXME
    struct {

    } queue;
};

} // namespace core
