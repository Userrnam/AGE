#pragma once

#include <vulkan/vulkan.h>

#include "Descriptor.hpp"
#include "Pool.hpp"

namespace age::core {

struct UniformBuffer {
    VkBuffer buffer;
    VkDeviceSize size;
};

struct Sampler {
    VkImageView view;
    VkSampler sampler;
};

struct DescriptorInfo {
    std::vector<UniformBuffer> ubos;
    uint32_t ubosBinding;
    std::vector<Sampler> samplers;
    uint32_t samplersBinding;
    uint32_t setCount = 16;
};

Descriptor getDescriptor(const DescriptorInfo& info);

// VkDescriptorPool requestDescriptorPool(uint32_t uboCount, uint32_t samplerCount, uint32_t setCount = 16);
VkDescriptorSetLayout requestDescriptorSetLayout(const DescriptorInfo& info);
VkDescriptorSet requestDescriptorSet(const DescriptorInfo& info, VkDescriptorSetLayout layout);

Pool& createDescriptorPool(uint32_t uboCount, uint32_t samplerCount, uint32_t setCount);
VkDescriptorSetLayout createDescriptorSetLayout(const DescriptorInfo& info);
VkDescriptorSet createDescriptorSets(const DescriptorInfo& info, VkDescriptorSetLayout layout);
    
} // namespace age::core
