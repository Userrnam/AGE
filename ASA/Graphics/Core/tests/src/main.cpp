#include <iostream>

#include "coreAPI.hpp"
#include "Window.hpp"

int main() {
    core::debugEnable(true);

    core::init("test", 0);

    core::window::create(100, 100, "test");

    core::pickPhysicalDevice();
    core::createLogicalDevice();
    core::createSwapchain();

    while (!core::window::closed()) {
        core::window::pollEvents();
    }

    core::destroy();

    return 0;
}
