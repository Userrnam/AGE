#pragma once

#include <vector>
#include <cstdint>
#include <string>

#define MAKE_VERSION(major, minor, patch) \
    (((major) << 22) | ((minor) << 12) | (patch))

namespace age::core {

enum SampleCount {
    SAMPLE_COUNT_1 = 1,
    SAMPLE_COUNT_2 = 2,
    SAMPLE_COUNT_4 = 4,
    SAMPLE_COUNT_8 = 8,
    SAMPLE_COUNT_16 = 16,
    SAMPLE_COUNT_32 = 32,
    SAMPLE_COUNT_64 = 64
};

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
        uint32_t version = MAKE_VERSION(1, 0, 0);
    } appInfo;

    struct {
        bool resizable = false;
        std::string title;
        int width = 0;
        int height = 0;
    } window;

    struct {
        SampleCount sampleCount = SAMPLE_COUNT_1;
    } multisampling;
    
// FIXME
    struct {
        bool compute = false;
    } queue;
};

} // namespace age::core
