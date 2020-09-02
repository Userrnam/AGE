#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace age::core {

typedef uint32_t RenderPassConfig;

enum RenderPassConfigBits {
    RENDER_PASS_NO_CONFIG = 0,
    RENDER_PASS_MULTISAMPLING_BIT = 1
};

struct RenderPassRef {
    std::vector<VkFramebuffer> framebuffers;
    VkRenderPass renderPass;
    RenderPassConfig config;

    void destroy();
};

} // namespace age::core
