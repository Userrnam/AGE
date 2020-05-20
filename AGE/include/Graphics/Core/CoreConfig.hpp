#pragma once

#include <vector>
#include <string>
#include <vulkan/vulkan.h>

namespace age::core {

struct CoreConfig {
    std::vector<const char*> deviceExtensions = { "VK_KHR_swapchain" };
    struct {
        bool geometryShader = false;
        bool tesselationShader = false;
        bool samplerAnistropy = false;
        bool sampleRateShading = false;
    } features;

    struct {
        std::string name;
        uint32_t version = VK_MAKE_VERSION(1, 0, 0);
    } appInfo;

    struct {
        bool resizable = false;
        std::string title;
        int width = 0;
        int height = 0;
    } window;

    struct {
        VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT;
    } multisampling;

    bool debugEnable = true;
};

} // namespace age::core
