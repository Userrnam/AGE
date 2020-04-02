#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "core/Window.hpp"
#include "core/VulkanCore.hpp"

namespace core {

void Window::init(int width, int height, const char *title) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_windowHandle = glfwCreateWindow(width, height, title, nullptr, nullptr);
    
    // get surface
    glfwCreateWindowSurface(core::getInstance(), (GLFWwindow*)m_windowHandle, nullptr, (VkSurfaceKHR*)&m_surfaceHandle);
}

Window::~Window() {
    glfwDestroyWindow((GLFWwindow*)m_windowHandle);
}

void Window::pollEvents() {
    glfwPollEvents();
}

bool Window::closed() {
    return glfwWindowShouldClose((GLFWwindow*)m_windowHandle);
}

} // namespace core
