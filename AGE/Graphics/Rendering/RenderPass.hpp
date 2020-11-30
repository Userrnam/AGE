#pragma once

#include <vulkan/vulkan.h>

namespace age {

class RenderPass {
    static VkRenderPass m_renderPass;
public:
    static void create();
    static void destroy();    
    inline static VkRenderPass get() {
        return m_renderPass;
    }
};

} // namespace age
