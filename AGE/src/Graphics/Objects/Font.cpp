#include <stdexcept>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Font.hpp"
#include "MemoryHolders/Buffer.hpp"

namespace age {

FT_Library ftLibrary;

void initFreeType() {
    if (FT_Init_FreeType(&ftLibrary)) {
        throw std::runtime_error("initFreeType: failed to init");
    }
}

void Font::load(const std::string& fontPath, unsigned fontSize, Sampler sampler) {
    FT_Face face;
    if (FT_New_Face(ftLibrary, fontPath.c_str(), 0, &face)) {
        throw std::runtime_error("Font::load: failed to load font");
    }

    if (FT_Set_Pixel_Sizes(face, 0, fontSize)) {
        throw std::runtime_error("Font::load: failed to set font size");
    }

    // FIXME: image is long now, may be there is a better way
    // to determine size of atlas
    unsigned maxHeight = 0;
    unsigned totalWidth = 0;
    for (unsigned char c = 0; c < 128; ++c) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            throw std::runtime_error("Font::load: failed to load Glyph");
        }
        totalWidth += face->glyph->bitmap.width;
        if (face->glyph->bitmap.rows > maxHeight) {
            maxHeight = face->glyph->bitmap.rows;
        }
    }
    // add spacing
    totalWidth += 128;

    float textureWidth = static_cast<float>(totalWidth);
    float textureHeight = static_cast<float>(maxHeight);

    char* pixels = new char[maxHeight * totalWidth];
    unsigned curX = 0;

    // fill texture atlas
    for (unsigned char c = 32; c < 127; ++c) {
        FT_Load_Char(face, c, FT_LOAD_RENDER);
        FT_Bitmap* bitmap = &face->glyph->bitmap;

        // copy bitmap
        for (unsigned row = 0; row < bitmap->rows; ++row) {
            for (unsigned col = 0; col < bitmap->width; ++col) {
                unsigned x = curX + col;
                unsigned y = row;
                pixels[y * totalWidth + x] = bitmap->buffer[row * bitmap->pitch + col];
            }
        }

        auto& character = m_characters[c];
        character.advance = face->glyph->advance.x >> 6;
        character.bearing.x = face->glyph->bitmap_left;
        character.bearing.y = face->glyph->bitmap_top;
        character.size.x = face->glyph->bitmap.width;
        character.size.y = face->glyph->bitmap.rows;

        float leftX = static_cast<float>(curX) / textureWidth;
        float rightX = static_cast<float>(curX + bitmap->width) / textureWidth;
        float lowY = static_cast<float>(bitmap->rows) / textureHeight;
        float highY = 0.0f;
        character.texCoords[0] = { leftX, lowY };
        character.texCoords[1] = { rightX, lowY };
        character.texCoords[2] = { rightX, highY };
        character.texCoords[3] = { leftX, highY };

        curX += bitmap->width + 1;
    }

    // FIXME:
    char* imageData = (char*)calloc(maxHeight * totalWidth, sizeof(uint32_t));
    for (size_t i = 0; i < maxHeight * totalWidth; ++i) {
        imageData[i * 4 + 0] |= pixels[i];
		imageData[i * 4 + 1] |= pixels[i];
		imageData[i * 4 + 2] |= pixels[i];
		imageData[i * 4 + 3] |= pixels[i];
    }
    delete [] pixels;

    Buffer stagingBuffer;
    stagingBuffer.create(
        BufferCreateInfo()
            .setUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
            .setSize(maxHeight * totalWidth * sizeof(uint32_t))
    );
    stagingBuffer.load(imageData, maxHeight * totalWidth * sizeof(uint32_t));
    free(imageData);

    core::Image image;
    image.create(
        core::ImageCreateInfo()
            .setAspectFlags(VK_IMAGE_ASPECT_COLOR_BIT)
            .setExtent({static_cast<uint32_t>(textureWidth), static_cast<uint32_t>(textureHeight)})
            .setFormat(VK_FORMAT_R8G8B8A8_SRGB)
            .setImageUsage(VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
            .setSampleCount(VK_SAMPLE_COUNT_1_BIT)
    );
    stagingBuffer.copyTo(image);
    stagingBuffer.destroy();

    m_atlas.create(image, sampler);
}

void Font::destroy() {
    m_atlas.destroy();
}

} // namespace age
