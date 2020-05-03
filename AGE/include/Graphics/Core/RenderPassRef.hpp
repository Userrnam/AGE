#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace age::core {

enum RenderPassConfig {
    RENDER_PASS_DEPTH_BIT,
    RENDER_PASS_MULTISAMPLING_BIT
};

struct RenderPassRef {
    std::vector<VkFramebuffer> framebuffers;
    VkRenderPass renderPass;
    RenderPassConfig config;

    void destroy();
};

} // namespace age::core
