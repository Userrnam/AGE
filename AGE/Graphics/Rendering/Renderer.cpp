#include <ft2build.h>
#include FT_FREETYPE_H

#include "Renderer.hpp"
#include "../Core/CoreCreator.hpp"
#include "../Core/Core.hpp"
#include "PipelineManager.hpp"
#include "Framebuffers.hpp"
#include "RenderPass.hpp"
#include "../ObjectCreation/DescriptorManager.hpp"

namespace age {


extern FT_Library ftLibrary;

inline void initFreetype() {
    if (FT_Init_FreeType(&ftLibrary)) {
        throw std::runtime_error("initFreeType: failed to init");
    }
}

std::vector<Drawable> Renderer::m_previousTargets;

void Renderer::render(const std::vector<Drawable>& targets) {
    m_previousTargets = targets;
    std::sort(m_previousTargets.begin(), m_previousTargets.end(), [](const Drawable& a, const Drawable& b) {
        return a.getZ() < b.getZ();
    });

    // update active commandBuffer
    if (core::apiCore.commandBuffers.active == core::apiCore.commandBuffers.data.data()) {
        core::apiCore.commandBuffers.active += core::apiCore.swapchain.images.size();
    } else {
        core::apiCore.commandBuffers.active -= core::apiCore.swapchain.images.size();
    }

    VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

    VkRenderPassBeginInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = RenderPass::get();
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = core::apiCore.swapchain.extent;

	for (size_t i = 0; i < core::apiCore.commandBuffers.size; ++i) {
        vkResetCommandBuffer(core::apiCore.commandBuffers.active[i], 0);
		if (vkBeginCommandBuffer(core::apiCore.commandBuffers.active[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer");
		}

        renderPassInfo.framebuffer = Framebuffers::get(i);

        vkCmdBeginRenderPass(core::apiCore.commandBuffers.active[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        // set viewport and scissors
        VkViewport viewport = {};
        viewport.width = core::apiCore.swapchain.extent.width;
        viewport.height = core::apiCore.swapchain.extent.height;

        VkRect2D scissors = {};
        scissors.extent.width = viewport.width;
        scissors.extent.height = viewport.height;

        vkCmdSetViewport(core::apiCore.commandBuffers.active[i], 0, 1, &viewport);
        vkCmdSetScissor(core::apiCore.commandBuffers.active[i], 0, 1, &scissors);

        for (auto& target : m_previousTargets) {
            target.draw(i);
        }

        vkCmdEndRenderPass(core::apiCore.commandBuffers.active[i]);

		if (vkEndCommandBuffer(core::apiCore.commandBuffers.active[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer");
		}
	}
}

void Renderer::create() {
    initFreetype();
    core::initCore();
    RenderPass::create();
    Framebuffers::create(RenderPass::get());
}

void Renderer::destroy() {
    DescriptorManager::destroy();
    destroyPipelineManager();
    Framebuffers::destroy();
    RenderPass::destroy();
    core::destroyCore();
    
    FT_Done_FreeType(ftLibrary);
}

} // namespace age
