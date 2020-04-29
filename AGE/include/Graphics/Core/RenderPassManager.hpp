#pragma once

#include <vulkan/vulkan.h>

namespace age::core {

enum RenderPassConfig {
    RENDER_PASS_DEPTH_BIT,
    RENDER_PASS_MULTISAMPLING_BIT
};

VkRenderPass requestRenderPass(RenderPassConfig rpc);

} // namespace age::core
