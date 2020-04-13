#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace core {

struct Core {
	struct {
		bool enable = true;
		VkDebugUtilsMessengerEXT messenger = VK_NULL_HANDLE;
	} debug;

    struct {
        GLFWwindow *handle = nullptr;
        VkSurfaceKHR surface = VK_NULL_HANDLE;
    } window;

    struct {
        // only 1 present Queue, because only 1 window is supported
        VkQueue presentQueue = VK_NULL_HANDLE;
        std::vector<VkQueue> graphicsQueues;
        std::vector<VkQueue> transferQueues;
        std::vector<VkQueue> computeQueues;
    } queues;

    struct {
        VkSwapchainKHR swapchain = VK_NULL_HANDLE;
        VkFormat format;
        VkExtent2D extent;
        std::vector<VkImage> images;
        std::vector<VkImageView> imageViews;
        std::vector<VkFramebuffer> framebuffers;
    } swapchain;

    struct {
        VkImage image;
        VkImageView imageView;
        VkDeviceMemory imageMemory;
    } depth;

    struct {
        VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT;
        VkImage image;
        VkImageView imageView;
        VkDeviceMemory imageMemory;
    } multisampling;

	VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;

    VkRenderPass renderPass = VK_NULL_HANDLE;
};

}
