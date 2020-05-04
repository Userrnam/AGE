#include <cmath>

#include "Texture.hpp"
#include "External/stb_image.h"
#include "Core/Buffer.hpp"
#include "Core/Core.hpp"

namespace age {

void Texture::create(const std::string& filename, const TextureCreateInfo& createInfo) {
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(filename.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    if (!pixels) {
        throw std::runtime_error("Texture::create: failed to load image " + filename);
    }

    VkDeviceSize imageSize = texWidth * texHeight * 4;

    uint32_t mipLevel = 1;
    if (createInfo.mipLevel == IMAGE_MIP_LEVEL_AUTO) {
        mipLevel = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;
    } else {
        mipLevel = createInfo.mipLevel;
    }

    core::BufferCreateInfo bufferCreateInfo;
    bufferCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    bufferCreateInfo.size = imageSize;
    bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

    core::Buffer stagingBuffer;
    stagingBuffer.create(bufferCreateInfo);
    stagingBuffer.loadData(pixels, imageSize);
    stbi_image_free(pixels);

    core::ImageCreateInfo imageCreateInfo;
    imageCreateInfo.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
    imageCreateInfo.extent.width = static_cast<uint32_t>(texWidth);
    imageCreateInfo.extent.height = static_cast<uint32_t>(texHeight);
    imageCreateInfo.format = core::apiCore.swapchain.format;
    imageCreateInfo.imageUsage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    imageCreateInfo.memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    imageCreateInfo.mipLevel = mipLevel;
    imageCreateInfo.numberOfSamples = core::apiCore.multisampling.sampleCount;
    imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;

    m_image.create(imageCreateInfo);
    stagingBuffer.copyTo(m_image);
    stagingBuffer.destroy();

    if (createInfo.mipLevel > 1) {
        m_image.generateMipmaps();
    }

    // create sampler
    VkSamplerCreateInfo samplerInfo = {};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = createInfo.sampler.magFilter;
    samplerInfo.minFilter = createInfo.sampler.minFilter;
    samplerInfo.addressModeU = createInfo.sampler.addressModeU;
    samplerInfo.addressModeW = createInfo.sampler.addressModeW;

    if (createInfo.sampler.maxAnistropy > 0.0f) {
        samplerInfo.anisotropyEnable = createInfo.sampler.maxAnistropy;
        samplerInfo.maxAnisotropy = createInfo.sampler.maxAnistropy;
    }

    samplerInfo.borderColor = VK_BORDER_COLOR_INT_TRANSPARENT_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;

    // ??
    // samplerInfo.compareEnable = VK_TRUE;
    // samplerInfo.compareOp = VK_COMPARE_OP_LESS;

    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = static_cast<float>(m_image.getMipLevel());
    samplerInfo.mipLodBias = 0.0f;

    if (vkCreateSampler(core::apiCore.device, &samplerInfo, nullptr, &m_sampler) != VK_SUCCESS) {
        throw std::runtime_error("failed to create sampler");
    }
}

void Texture::destroy() {
    m_image.destroy();
    vkDestroySampler(core::apiCore.device, m_sampler, nullptr);
}

} // namespace age
