#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include <iostream>
#include <GLFW/glfw3.h>

#include "MemoryHolders/Image.hpp"
#include "Rendering/RenderPassManager.hpp"
#include "Rendering/RenderPassRef.hpp"
#include "ObjectCreation/DescriptorLayout.hpp"
#include "Pool.hpp"

#define printVar(s, ident) for (int i=0;i<ident;++i) { std::cout << ' '; } std::cout << #s << ": " << s << "\n"

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
        std::vector<Pool> pools;
        std::vector<DescriptorLayout> layouts;
    } descriptor;

    struct {
        VkSemaphore imageAvailableSemaphore;
        VkSemaphore renderFinishedSemaphore;
        VkFence inFlightFence;
        // std::vector<VkFence> imagesInFlight;
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

    RenderPassRef renderPass;
};

extern Core apiCore;

} // namespace age::core
