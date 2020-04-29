#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "RenderPassManager.hpp"

namespace age::core {

struct RenderPassRef {
    std::vector<VkFramebuffer> framebuffers;
    VkRenderPass renderPass;
    RenderPassConfig config;

    void destroy();
};

} // namespace age::core
