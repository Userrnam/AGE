#include <cmath>

#include "Texture.hpp"
#include "../../External/stb_image.h"
#include "../MemoryHolders/Buffer.hpp"
#include "../Core/Core.hpp"
#include "../Core/Utils/TransitionImageLayout.hpp"

namespace age {

void Texture::create(const std::string& filename, Sampler sampler) {
    m_sampler = sampler;

    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(filename.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    if (!pixels) {
        throw std::runtime_error("Texture::create: failed to load image " + filename);
    }

    VkDeviceSize imageSize = texWidth * texHeight * 4;

    Buffer stagingBuffer;
    stagingBuffer.create(
        BufferCreateInfo()
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
            .setSampleCount(VK_SAMPLE_COUNT_1_BIT)
    );
    stagingBuffer.copyTo(m_image);
    stagingBuffer.destroy();
}

void Texture::create(const core::Image& image, Sampler sampler) {
    m_image = image;
    m_sampler = sampler;
}

void Texture::destroy() {
    m_image.destroy();
}

} // namespace age
