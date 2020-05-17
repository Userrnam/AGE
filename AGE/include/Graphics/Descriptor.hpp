#pragma once

#include <vulkan/vulkan.h>
#include <vector>

#include "Core/Buffer.hpp"
#include "Texture.hpp"

namespace age {

// ubos - binding 0
// samplers - binding 1
struct DescriptorInfo {
    std::vector<core::Buffer*> ubos;
    std::vector<Texture*> textures;
    uint32_t setCount = 16;
};

class Descriptor {
    void* m_pool;
    VkDescriptorSetLayout m_layout;
    VkDescriptorSet m_set;

    VkDescriptorSet requestDescriptorSet(const DescriptorInfo& info);
    VkDescriptorSet createDescriptorSets(const DescriptorInfo& info);

    friend class Object;
public:
    VkDescriptorSetLayout getLayout() { return m_layout; }
    VkDescriptorSet getSet() { return m_set; }

    void get(DescriptorInfo& info);
};

void freeDescriptor(void* pool, void* descriptor);

} // namespace age
