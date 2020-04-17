#include <iostream>

#include "coreAPI.hpp"
#include "Window.hpp"
#include "Command.hpp"

void fillCmd(int i) {
    core::cmd::clear(i);
}

int main() {
    core::debugEnable(true);

    core::init("test", 0);

    core::window::create(100, 100, "test");

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
    core::fillCommandBuffers(fillCmd);

    while (!core::window::closed()) {
        core::window::pollEvents();
        core::window::present();
    }

    core::destroy();

    return 0;
}
