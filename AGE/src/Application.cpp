#include "Application.hpp"

#include "coreAPI.hpp"
#include "Window.hpp"
#include "Core.hpp"
#include "Command.hpp"

namespace age {

void Application::updateCommandBuffers() {
    // update active commandBuffer
    if (core::apiCore.commandBuffers.active == core::apiCore.commandBuffers.data.data()) {
        core::apiCore.commandBuffers.active += core::apiCore.swapchain.framebuffers.size();
    } else {
        core::apiCore.commandBuffers.active -= core::apiCore.swapchain.framebuffers.size();
    }

    VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

	for (size_t i = 0; i < core::apiCore.commandBuffers.size; ++i) {
        vkResetCommandBuffer(core::apiCore.commandBuffers.active[i], 0);
		if (vkBeginCommandBuffer(core::apiCore.commandBuffers.active[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer");
		}

		draw(i);

		if (vkEndCommandBuffer(core::apiCore.commandBuffers.active[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer");
		}
	}
}

void Application::draw(int i) {
    core::cmd::clear(i);
}

void Application::onConfigure() {
    core::debugEnable(true);
    core::setClearColor({1, 1, 1, 1});
}

void Application::onCreate() {
    updateCommandBuffers();
}

Application::~Application() {
    core::destroy();
}

void Application::init() {
    onConfigure();

    core::init();
    core::window::create();

    core::pickPhysicalDevice();
    core::createLogicalDevice();
    core::createSwapchain();
    core::createRenderPass();
    core::createDepthResources();
    core::createMultisamplingResources();
    core::createFramebuffers();
    core::createCommandPools();
    core::createDescriptorPool();
    core::createDescriptorSetLayout();
    core::createSyncObjects();

    core::allocateCommandBuffers();
    core::createCamera();

    onCreate();
}

void Application::run() {
    while (!core::window::closed()) {
        core::window::pollEvents();
        onUpdate();
        core::window::present();
    }
}

} // namespace age
