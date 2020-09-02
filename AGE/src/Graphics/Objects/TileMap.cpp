#include "TileMap.hpp"

#include "ObjectCreation/ShapeManager.hpp"

#ifndef CMAKE_DEFINITION
#define SHADER_PATH ""
#endif

namespace age {

void TileMap::create(Texture& texture, uint32_t maxSize, bool blendEnable) {
    m_maxSize = maxSize;
    m_tiles.reserve(m_maxSize);

    DrawableCreateInfo createInfo;

    m_buffer.create(
        BufferCreateInfo()
        .setUsage(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT)
        .setSize(sizeof(Tile) * m_maxSize + sizeof(MapData))
    );

    Shader vertexShader;
    Shader fragmentShader;

    vertexShader
        .setStage(VK_SHADER_STAGE_VERTEX_BIT)
        .create(SHADER_PATH "TileMap.vert.spv");

    fragmentShader.setStage(VK_SHADER_STAGE_FRAGMENT_BIT).create(SHADER_PATH "TileMap.frag.spv");

    Drawable::create(
        DrawableCreateInfo()
        .setColorBlendEnable(blendEnable)
        .setIstanceCount(0)
        .addDescriptorSet(
            DescriptorSet().get(
                DescriptorSetInfo()
                .addBinding(
                    DescriptorBinding()
                    .add(m_buffer)
                    .setStage(VK_SHADER_STAGE_VERTEX_BIT)
                    .setDescriptorType(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
                )
                .addBinding(
                    DescriptorBinding()
                    .add(texture)
                    .setDescriptorType(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
                    .setStage(VK_SHADER_STAGE_FRAGMENT_BIT)
                )
            )
        )
        .setShapeId(RECTANGLE_SHAPE)
        .addShader(vertexShader)
        .addShader(fragmentShader)
    );

    vertexShader.destroy();
    fragmentShader.destroy();
}

void TileMap::destroy() {
    m_buffer.destroy();
    freeDescriptor(m_poolIndicies[1], m_descriptorSets[1]);
    destroyPipeline(m_pipeline);
}

void TileMap::setColor(const glm::vec4& color) {
    m_mapData.color = color;
}

void TileMap::setColor(float r, float g, float b, float a) {
    m_mapData.color.r = r;
    m_mapData.color.g = g;
    m_mapData.color.b = b;
    m_mapData.color.a = a;
}

void TileMap::addTile(const Tile& tile) {
    m_tiles.push_back(tile);
    m_instanceCount++;
}

void TileMap::removeTile(size_t index) {
    m_tiles.erase(m_tiles.begin() + index);
    m_instanceCount--;
}

void TileMap::clearTiles() {
    m_tiles.clear();
    m_instanceCount = 0;
}

void TileMap::uploadMapData() {
    m_mapData.transform = getTransform();
    m_buffer.load(&m_mapData, sizeof(MapData));
}

void TileMap::uploadTiles() {
    m_buffer.load(m_tiles.data(), m_tiles.size() * sizeof(Tile), sizeof(MapData));
}

} // namespace age