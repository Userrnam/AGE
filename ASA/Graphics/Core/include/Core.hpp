#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

struct Core {
	struct {
		bool enable = true;
		VkDebugUtilsMessengerEXT messenger = VK_NULL_HANDLE;
	} debug;

    struct {
        GLFWwindow *handle = nullptr;
        VkSurfaceKHR surface = VK_NULL_HANDLE;
    } window;

// FIXME: add multiple queues support
    struct {
        VkQueue presentQueue = VK_NULL_HANDLE;
        VkQueue graphicsQueue = VK_NULL_HANDLE;
        VkQueue transferQueue = VK_NULL_HANDLE;
    } queues;

	VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
};
