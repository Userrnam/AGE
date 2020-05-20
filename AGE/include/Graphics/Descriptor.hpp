#pragma once

#include <vulkan/vulkan.h>
#include <vector>

#include "Buffer.hpp"
#include "Texture.hpp"

namespace age {

// ubos - binding 0
// samplers - binding 1

class DescriptorInfo {
    std::vector<Buffer*> m_ubos;
    std::vector<Texture*> m_textures;
    uint32_t m_setCount = 16;

    friend class Descriptor;
    friend VkDescriptorSetLayout createDescriptorSetLayout(const DescriptorInfo& info);
    friend VkDescriptorSetLayout requestDescriptorSetLayout(const DescriptorInfo& info);
public:

    inline DescriptorInfo& addBuffer(Buffer& buffer) {
        m_ubos.push_back(&buffer);
        return *this;
    }

    inline DescriptorInfo& addTexture(Texture& texture) {
        m_textures.push_back(&texture);
        return *this;
    }

    inline DescriptorInfo& setSetCount(uint32_t count) {
        m_setCount = count;
        return *this;
    }
};

class Descriptor {
    void* m_pool;
    VkDescriptorSetLayout m_layout;
    VkDescriptorSet m_set;

    VkDescriptorSet requestDescriptorSet(const DescriptorInfo& info);
    VkDescriptorSet createDescriptorSets(const DescriptorInfo& info);

    friend class Drawable;
public:
    VkDescriptorSetLayout getLayout() { return m_layout; }
    VkDescriptorSet getSet() { return m_set; }

    Descriptor& get(const DescriptorInfo& info);
};

void freeDescriptor(void* pool, void* descriptor);

} // namespace age
