#pragma once

#include <vulkan/vulkan.h>
#include <vector>

namespace age {

class Framebuffers {
    static std::vector<VkFramebuffer> m_framebuffers;
public:
    static void create(VkRenderPass renderPass);
    static void destroy();

    inline static VkFramebuffer get(int i) {
        return m_framebuffers[i];
    }
};

} // namespace age
