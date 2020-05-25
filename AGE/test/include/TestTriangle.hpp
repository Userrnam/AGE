#pragma once

#include "Graphics.hpp"
#include "Utils/utils.hpp"
// #include "G"

struct VertexElement {
    glm::vec4 m_color;
    glm::vec2 m_coord;

    VertexElement(glm::vec4 color, glm::vec2 coord) : m_color(color), m_coord(coord) {}
};

std::vector<age::Vertex<VertexElement>> verticies = {
    VertexElement({ 0, 0, 1, 0 }, { 200, 0 }),
    VertexElement({ 0, 1, 0, 0 }, { 0, 300 }),
    VertexElement({ 1, 0, 0, 0 }, { 0, 0 }),
};

VERTEX_ATTRIBUTES(VertexElement) = {
    VK_FORMAT_R32G32B32A32_SFLOAT,
    VK_FORMAT_R32G32_SFLOAT
};

std::vector<age::Index16> indicies = {
    0, 1, 2
};

class TestTriangle : public age::Drawable {
    age::Buffer ubo;
    glm::vec4 blendColor;
public:
    void create(age::Layer* layer) {
        ubo.create(
            age::UniformBufferCreateInfo()
            .setSize(sizeof(blendColor))
        );

        blendColor = {1, 0, 0, 1};
        ubo.load(&blendColor, sizeof(blendColor));

        age::Shader vertexShader;
        age::Shader fragmentShader;

        vertexShader.setStage(VK_SHADER_STAGE_VERTEX_BIT).create(age::getResourcePath("test.vert.spv"));
        fragmentShader.setStage(VK_SHADER_STAGE_FRAGMENT_BIT).create(age::getResourcePath("test.frag.spv"));

        createDrawable(
            age::DrawableCreateInfo()
            .setColorBlendEnable(false)
            .setMultisamplingEnable(false)
            .setDepthTestEnable(false)
            .setMinSampleShading(0.0f)
            .setIstanceCount(1)
            .setLayer(layer)
            .loadIndicies(indicies)
            .loadVerticies(verticies)
            .addDescriptor(
                age::Descriptor().get(
                    age::DescriptorInfo()
                    .addBuffersBinding(
                        age::BuffersBinding().addBuffer(ubo).setStage(VK_SHADER_STAGE_FRAGMENT_BIT)
                    )
                )
            )
            .addShader(vertexShader)
            .addShader(fragmentShader)
        );

        vertexShader.destroy();
        fragmentShader.destroy();
    }

    void destroy() {
        m_vertex.buffer.destroy();
        m_index.buffer.destroy();
        ubo.destroy();
        age::freeDescriptor(m_poolIndicies[1], m_descriptorSets[1]);
        age::destroyPipeline(m_pipeline);
    }
};
