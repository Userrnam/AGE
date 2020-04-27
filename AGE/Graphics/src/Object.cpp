#include <vulkan/vulkan.h>

#include "Core.hpp"

#include "Object.hpp"
#include "Vertex.hpp"


namespace age {

/*

Pipeline requires:
1. Shaders
2. binding descriptions
3. depth test
4. viewport
5. minSampleRateShading (if multisampling enabled)
6. color blending
7. pipeline layout -> descriptorSetLayouts
8. render pass

NOTE:
shader modules are not required after pipeline is created

*/

void Object::create(ObjectCreateInfo& info) {
    
}

void Object::draw(int i) {
    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = core::apiCore.renderPass;
    renderPassInfo.framebuffer = core::apiCore.swapchain.framebuffers[i];
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = core::apiCore.swapchain.extent;

    VkBuffer vertexBuffers[] = { (VkBuffer)vertexBuffer };
    VkDeviceSize offsets[] = { 0 };

    vkCmdBeginRenderPass(core::apiCore.commandBuffers.active[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(core::apiCore.commandBuffers.active[i], VK_PIPELINE_BIND_POINT_GRAPHICS, (VkPipeline)pipeline);
        vkCmdBindVertexBuffers(core::apiCore.commandBuffers.active[i], 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(core::apiCore.commandBuffers.active[i], (VkBuffer)indexBuffer, 0, VK_INDEX_TYPE_UINT32);
        // vkCmdBindDescriptorSets(core::apiCore.commandBuffers.active[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[i], 0, nullptr);
        vkCmdDrawIndexed(core::apiCore.commandBuffers.active[i], indexCount, 1, 0, 0, 0);
    vkCmdEndRenderPass(core::apiCore.commandBuffers.active[i]);

    if (vkEndCommandBuffer(core::apiCore.commandBuffers.active[i]) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer");
    }
}

}
