#include "RenderPassRef.hpp"
#include "Core/Core.hpp"

namespace age {

void RenderPassRef::destroy() {
    vkDestroyRenderPass(core::apiCore.device, renderPass, nullptr);
    for (auto framebuffer : framebuffers) {
        vkDestroyFramebuffer(core::apiCore.device, framebuffer, nullptr);
    }
}
    
} // namespace age

