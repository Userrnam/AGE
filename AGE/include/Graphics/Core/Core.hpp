#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include <iostream>
#include <GLFW/glfw3.h>

#include "Image.hpp"
#include "RenderPassManager.hpp"
#include "RenderPassRef.hpp"
#include "DescriptorLayout.hpp"
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
        VkClearColorValue clearColor = {};
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

    bool framebufferResized = false;

	VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;

    std::vector<RenderPassRef> renderPasses;

    void print(int tab = 4) {
        std::cout << "descriptor.pools:\n";
        int ident = 0;
        ident += tab;
        for (auto& pool : descriptor.pools) {
            printVar(pool.layout, ident);
            printVar(pool.pool, ident);
            printVar(pool.remainingSize, ident);

            for (int i=0;i<tab*2;++i) { std::cout << ' '; }
            std::cout << "sets\n";
            ident += tab;
            for (auto& set: pool.sets) {
                printVar(set, ident);
            }
            ident -= tab;
        }
        ident -= tab;

        std::cout << "descripotr.layouts:\n";
        ident += tab;
        for (auto& layout: descriptor.layouts) {
            printVar(layout.layout, ident);
            printVar(layout.samplerBinding, ident);
            printVar(layout.samplerCount, ident);
            printVar(layout.uboBinding, ident);
            printVar(layout.uboCount, ident);
        }
        ident -= tab;
    }
};

extern Core apiCore;

} // namespace age::core
