#include <iostream>

#include "coreAPI.hpp"
#include "Window.hpp"

int main() {
    core::debugEnable(true);
    core::init("test", 0);

    core::window::create(100, 100, "test");

    core::DeviceRequirements requirements;
    core::pickPhysicalDevice(requirements);

    while (!core::window::closed()) {
        core::window::pollEvents();
    }

    core::destroy();

    return 0;
}
