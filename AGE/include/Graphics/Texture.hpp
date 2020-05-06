#pragma once

#include <string>
#include <vulkan/vulkan.h>

#include "Core/Image.hpp"

namespace age {

#define IMAGE_MIP_LEVEL_AUTO UINT32_MAX

struct TextureCreateInfo {
    uint32_t mipLevel = 1;
    struct {
        VkFilter magFilter = VK_FILTER_LINEAR;
        VkFilter minFilter = VK_FILTER_LINEAR;
        VkSamplerAddressMode addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        VkSamplerAddressMode addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        float maxAnistropy = 0.0f;
    } sampler;
};

class Texture {
    VkSampler m_sampler = VK_NULL_HANDLE;
    core::Image m_image;
public:
    ~Texture();
    // FIXME
    VkSampler getSampler() const { return m_sampler; }
    const core::Image& getImage() const { return m_image; }

    void create(const std::string& filename, const TextureCreateInfo& createInfo = TextureCreateInfo());
};

} // namespace age
