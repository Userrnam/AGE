#pragma once

#include "Graphics.hpp"
#include "Utils/utils.hpp"

struct VertexElement {
    glm::vec4 m_color;
    glm::vec2 m_coord;

    VertexElement(glm::vec4 color, glm::vec2 coord) : m_color(color), m_coord(coord) {}
};

std::vector<age::Vertex<VertexElement>> verticies = {
    VertexElement({ 0, 0, 1, 0 }, { 2 * 800, 0 }),
    VertexElement({ 1, 0, 0, 0 }, { 2 * 800, 2 * 600 }),
    VertexElement({ 0, 1, 0, 0 }, { 0, 2 * 600 }),
};

VERTEX_ATTRIBUTES(VertexElement) = {
    VK_FORMAT_R32G32B32A32_SFLOAT,
    VK_FORMAT_R32G32_SFLOAT
};

std::vector<age::Index16> indicies = {
    0, 1, 2
};

class TestTriangle : public age::Drawable {
    age::ShapeId shapeId;
    age::Buffer ubo;
    glm::vec4 blendColor;
public:
    void create() {
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

        shapeId = age::Shape::create(
            age::ShapeCreateInfo().loadIndicies(indicies).loadVerticies(verticies)
        );

        Drawable::create(
            age::DrawableCreateInfo()
            .setColorBlendEnable(false)
            .setIstanceCount(1)
            .setShapeId(shapeId)
            .addDescriptorSet(
                age::DescriptorSet().get(
                    age::DescriptorSetInfo()
                    .addBinding(
                        age::DescriptorBinding()
                        .add(ubo)
                        .setStage(VK_SHADER_STAGE_FRAGMENT_BIT)
                        .setDescriptorType(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
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
        ubo.destroy();
        age::freeDescriptor(m_poolIndicies[1], m_descriptorSets[1]);
        age::destroyPipeline(m_pipeline);
    }
};
