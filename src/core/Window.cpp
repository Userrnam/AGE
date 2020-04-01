#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "core/Window.hpp"
#include "core/AppProperties.hpp"

namespace core {

void Window::init(int width, int height, const char *title) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_windowHandle = glfwCreateWindow(width, height, title, nullptr, nullptr);
}

Window::~Window() {
    glfwDestroyWindow((GLFWwindow*)m_windowHandle);
    glfwTerminate();
}

} // namespace core
