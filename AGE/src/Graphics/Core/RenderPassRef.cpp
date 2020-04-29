#include "RenderPassRef.hpp"
#include "Core.hpp"

namespace age::core {

void RenderPassRef::destroy() {
    vkDestroyRenderPass(apiCore.device, renderPass, nullptr);
    for (auto framebuffer : framebuffers) {
        vkDestroyFramebuffer(apiCore.device, framebuffer, nullptr);
    }
}
    
} // namespace age::core

