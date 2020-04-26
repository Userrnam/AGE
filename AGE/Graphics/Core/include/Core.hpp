#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "coreAPI.hpp"
#include "Image.hpp"

namespace age::core {

struct Queue {
    uint32_t index;
    VkQueue queue;
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
        // graphics also used for present
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
        std::vector<VkFramebuffer> framebuffers;
        VkClearColorValue clearColor = {};
    } swapchain;

    Image depthImage;

    struct {
        VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT;
        Image image;
    } multisampling;

    struct {
        VkCommandPool transferPool;
        VkCommandPool graphicsPool;
    } commandPools;

    struct {
        struct DPU {
            VkDescriptorPool pool;
            DescriptorUsage usage;
        };
        struct DLU {
            VkDescriptorSetLayout layout;
            uint32_t uboCount;
            uint32_t samplerCount;
        };

        std::vector<DPU> pools;
        std::vector<DLU> layouts;
    } descriptor;

    struct {
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        std::vector<VkFence> imagesInFlight;
        uint32_t currentFrame = 0;
    } sync;

    struct {
        std::vector<VkDescriptorSet> descriptors;
        std::vector<VkBuffer> buffers;
        std::vector<VkDeviceMemory> buffersMemory;
    } camera;

    struct {
        std::vector<VkCommandBuffer> data;
        VkCommandBuffer* active = nullptr;
        uint32_t size = 0;
    } commandBuffers;

    bool framebufferResized = false;

	VkInstance instance = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;

    VkRenderPass renderPass = VK_NULL_HANDLE;
};

extern Core apiCore;

} // namespace age::core
