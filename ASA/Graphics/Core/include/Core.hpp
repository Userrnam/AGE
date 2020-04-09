#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

struct Core {
	struct {
		bool enable = true;
		VkDebugUtilsMessengerEXT messenger;
	} debug;

    struct {
        GLFWwindow *handle = nullptr;
        VkSurfaceKHR surface;
    } window;

	VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
};
