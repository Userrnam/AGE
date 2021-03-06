#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "../MemoryHolders/Image.hpp"
#include "../ObjectCreation/DescriptorLayout.hpp"
#include "Pool.hpp"

namespace age::core {

struct Queue {
    uint32_t index;
    VkQueue queue;
};

struct PipelineLayoutRef {
    std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
    VkPipelineLayout pipelineLayout;
};

struct Core {
	struct {
		bool enable = true;
		VkDebugUtilsMessengerEXT messenger = VK_NULL_HANDLE;
	} debug;

    struct {
        GLFWwindow *handle = nullptr;
        VkSurfaceKHR surface = VK_NULL_HANDLE;
        int width = 0;
        int height = 0;
    } window;

    struct {
        Queue present;
        Queue graphics;
        Queue transfer;
        Queue compute;
    } queues;

    struct {
        VkSwapchainKHR swapchain = VK_NULL_HANDLE;
        VkFormat format;
        VkExtent2D extent;
        std::vector<VkImage> images;
        std::vector<VkImageView> imageViews;
    } swapchain;

    struct {
        Image image;
        VkFormat format;
    } depth;

    struct {
        VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT;
        Image image;
    } multisampling;

    struct {
        VkCommandPool transferPool;
        VkCommandPool graphicsPool;
    } commandPools;

    struct {
        VkSemaphore imageAvailableSemaphore;
        VkSemaphore renderFinishedSemaphore;
        VkFence inFlightFence;
    } sync;

    std::vector<PipelineLayoutRef> pipelineLayouts;

    struct {
        std::vector<VkCommandBuffer> data;
        VkCommandBuffer* active = nullptr;
        uint32_t size = 0;
    } commandBuffers;

    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;

    bool framebufferResized = false;
    
	VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
};

extern Core apiCore;

} // namespace age::core
