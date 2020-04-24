#include <iostream>

#include "coreAPI.hpp"
#include "Window.hpp"
#include "Command.hpp"

void fillCmd(int i) {
    age::core::cmd::clear(i);
}

int main() {
    age::core::debugEnable(true);

    age::core::init("test", 0);

    age::core::window::create(100, 100, "test");

    age::core::pickPhysicalDevice();
    age::core::createLogicalDevice();
    age::core::createSwapchain();
    age::core::createRenderPass();
    age::core::createDepthResources();
    age::core::createMultisamplingResources();
    age::core::createFramebuffers();
    age::core::createCommandPools();
    age::core::createDescriptorPool();
    age::core::createDescriptorSetLayout();
    age::core::createSyncObjects();

    age::core::allocateCommandBuffers();

    age::core::setClearColor({1, 1, 0, 1});
    age::core::fillCommandBuffers(fillCmd);

    while (!age::core::window::closed()) {
        age::core::window::pollEvents();
        age::core::window::present();
    }

    age::core::destroy();

    return 0;
}
