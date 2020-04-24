#pragma once

#include <vector>

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
        bool resizable = false;
    } window;

    struct {
        SampleCount sampleCount = SAMPLE_COUNT_1;
    } multisampling;

    uint32_t maxFramesInFlight = 1;
    
// FIXME
    struct {
        bool compute = false;
    } queue;
};

} // namespace age::core
