#include <cmath>

#include "Texture.hpp"
#include "External/stb_image.h"
#include "Core/Buffer.hpp"
#include "Core/Core.hpp"
#include "Core/TransitionImageLayout.hpp"

namespace age {

void Texture::create(const TextureCreateInfo& createInfo) {
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

	core::TransitionInfo transitionInfo;
	transitionInfo.image = m_image.getImage();
    transitionInfo.mipLevel = m_image.getMipLevel();
	transitionInfo.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	transitionInfo.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    transitionInfo.srcAccessMask = 0;
	transitionInfo.dstAccessMask = 0;
	transitionInfo.srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	transitionInfo.dstStage = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
	transitionImageLayout(transitionInfo);

    if (vkCreateSampler(core::apiCore.device, &samplerInfo, nullptr, &m_sampler) != VK_SUCCESS) {
        throw std::runtime_error("failed to create sampler");
    }
}

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

    core::Buffer stagingBuffer;
    stagingBuffer.create(
        core::BufferCreateInfo()
            .setMemoryProperties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
            .setSize(imageSize)
            .setUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
    );
    stagingBuffer.load(pixels, imageSize);
    stbi_image_free(pixels);

    m_image.create(
        core::ImageCreateInfo()
            .setAspectFlags(VK_IMAGE_ASPECT_COLOR_BIT)
            .setExtent({static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight)})
            .setFormat(VK_FORMAT_R8G8B8A8_SRGB)
            .setImageUsage(VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
            .setMemoryProperties(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
            .setMipLevel(mipLevel)
            .setSampleCount(VK_SAMPLE_COUNT_1_BIT)
    );
    stagingBuffer.copyTo(m_image);
    stagingBuffer.destroy();

    // FIXME: image should be loaded mipmaped
    if (createInfo.mipLevel > 1) {
        m_image.generateMipmaps();
    }

    // create sampler
    this->create(createInfo);
}

void Texture::destroy() {
    m_image.destroy();
    vkDestroySampler(core::apiCore.device, m_sampler, nullptr);
}

} // namespace age
