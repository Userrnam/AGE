#pragma once

#include "Graphics.hpp"
#include "Utils/utils.hpp"

std::vector<age::Vertex> verticies = {
    { 800, 0 },
    { 800, 600 },
    { 0, 600 }
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
