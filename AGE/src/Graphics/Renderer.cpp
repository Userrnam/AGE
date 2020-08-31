#include "Renderer.hpp"
#include "Graphics/Core/Command.hpp"
#include "Graphics/Core/CoreCreator.hpp"
#include "Graphics/Core/Core.hpp"
#include "Core/PipelineManager.hpp"

namespace age {

void Renderer::render(const std::vector<Drawable>& targets) {
    // TODO: update only if something changed

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
    renderPassInfo.renderPass = core::apiCore.renderPass.renderPass;
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = core::apiCore.swapchain.extent;

	for (size_t i = 0; i < core::apiCore.commandBuffers.size; ++i) {
        vkResetCommandBuffer(core::apiCore.commandBuffers.active[i], 0);
		if (vkBeginCommandBuffer(core::apiCore.commandBuffers.active[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer");
		}

        renderPassInfo.framebuffer = core::apiCore.renderPass.framebuffers[i];

        vkCmdBeginRenderPass(core::apiCore.commandBuffers.active[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        for (auto& target : targets) {
            target.draw(i);
        }

        vkCmdEndRenderPass(core::apiCore.commandBuffers.active[i]);

		if (vkEndCommandBuffer(core::apiCore.commandBuffers.active[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer");
		}
	}
}

void Renderer::create() {
    core::initCore();

    // clear screen
    VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

	for (size_t i = 0; i < core::apiCore.commandBuffers.size; ++i) {
        vkResetCommandBuffer(core::apiCore.commandBuffers.active[i], 0);
		if (vkBeginCommandBuffer(core::apiCore.commandBuffers.active[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer");
		}

        core::cmd::clear(i, {1,0,0,1});

		if (vkEndCommandBuffer(core::apiCore.commandBuffers.active[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer");
		}
	}
}

void Renderer::destroy() {
    core::destroyPipelineManager();
    core::destroyCore();
}

} // namespace age
