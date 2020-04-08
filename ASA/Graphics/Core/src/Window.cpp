#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>

#include "Window.hpp"
#include "Core.hpp"

extern Core apiCore;

namespace core {
namespace window {

void create(int width, int height, const char *title) {
    if (apiCore.window.handle) {
        throw std::runtime_error("ASA supports only one window");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    apiCore.window.handle = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (glfwCreateWindowSurface(apiCore.instance, apiCore.window.handle, nullptr, &apiCore.window.surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create surface");
    }
}

void pollEvents() {
    glfwPollEvents();
}

bool closed() {
    return glfwWindowShouldClose(apiCore.window.handle);
}

} // namespace window
} // namespace core
