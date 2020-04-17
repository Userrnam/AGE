#include <vulkan/vulkan.h>
#include <stdexcept>

#include "Texture.hpp"

#include "stb_image.h"


void Texture::load(const char* filename) {
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(filename, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

    if (!pixels) {
        std::runtime_error("failed to load image");
    }

    
}
