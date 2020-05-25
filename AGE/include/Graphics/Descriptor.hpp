#pragma once

#include <vulkan/vulkan.h>
#include <vector>

#include "Buffer.hpp"
#include "Texture.hpp"

#include "Debug.hpp"

namespace age {

class BuffersBinding {
    std::vector<Buffer*> m_ubos;
    VkShaderStageFlags m_stage = 0;

public:

#ifdef DEBUG
    bool stageSet = false;
#endif

    inline BuffersBinding& addBuffer(Buffer& buffer) {
        m_ubos.push_back(&buffer);
        return *this;
    }

    inline BuffersBinding& setStage(VkShaderStageFlags stage) {
#ifdef DEBUG
        stageSet = true;
#endif
        m_stage = stage;
        return *this;
    }

    inline VkShaderStageFlags getStage() const { return m_stage; }
    inline const std::vector<Buffer*>& getBuffers() const { return m_ubos; }
};

class TexturesBinding {
    std::vector<Texture*> m_textures;
public:
    inline TexturesBinding& addTexture(Texture& texture) {
        m_textures.push_back(&texture);
        return *this;
    }

    inline const std::vector<Texture*>& getTextures() const { return m_textures; }
};

class DescriptorInfo {
    std::vector<BuffersBinding> m_ubosBindings;
    std::vector<TexturesBinding> m_texturesBindings;
    uint32_t m_setCount = 16;

    friend class Descriptor;
    friend VkDescriptorSetLayout createDescriptorSetLayout(const DescriptorInfo& info);
    friend VkDescriptorSetLayout requestDescriptorSetLayout(const DescriptorInfo& info);
public:

    inline DescriptorInfo& addBuffersBinding(const BuffersBinding& buffersBinding) {
        ASSERT(buffersBinding.getBuffers().size(), "DescriptorInfo::addBuffersBinding: Binding has no buffers")
        ASSERT(buffersBinding.stageSet, "DescriptorInfo::addBuffersBinding: stage has not been set")

        m_ubosBindings.push_back(buffersBinding);
        return *this;
    }

    inline DescriptorInfo& addTexturesBinding(const TexturesBinding& texturesBinding) {
        ASSERT(texturesBinding.getTextures().size(), "DescriptorInfo::addTexturesBinding: binding has no textures")
        m_texturesBindings.push_back(texturesBinding);
        return *this;
    }

    inline DescriptorInfo& setSetCount(uint32_t count) {
        m_setCount = count;
        return *this;
    }
};

class Descriptor {
    uint32_t m_poolIndex = 0;
    VkDescriptorSetLayout m_layout = VK_NULL_HANDLE;
    VkDescriptorSet m_set = VK_NULL_HANDLE;

    VkDescriptorSet requestDescriptorSet(const DescriptorInfo& info);
    VkDescriptorSet createDescriptorSets(const DescriptorInfo& info);

    friend class Drawable;
public:
    VkDescriptorSetLayout getLayout() { return m_layout; }
    VkDescriptorSet getSet() { return m_set; }

    Descriptor& get(const DescriptorInfo& info);
};

void freeDescriptor(uint32_t poolIndex, VkDescriptorSet descriptor);

} // namespace age
