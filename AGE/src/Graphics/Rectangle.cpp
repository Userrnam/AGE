#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Rectangle.hpp"
#include "Core/Core.hpp"
#include "Shader.hpp"
#include "Vertex.hpp"
#include "Index.hpp"

#ifndef CMAKE_DEFINITION
#define SHADER_PATH ""
#endif

namespace age {

typedef glm::vec2 VType;

static std::vector<Vertex<VType>> verticies = {
    VType({ 0.0, 0.0 }),
    VType({ 1.0, 0.0 }),
    VType({ 1.0, 1.0 }),
    VType({ 0.0, 1.0 }),
};

VERTEX_ATTRIBUTES(VType) = {
    VK_FORMAT_R32G32_SFLOAT
};

static std::vector<Index16> indicies = { 0, 1, 2, 2, 3, 0 };

void Rectangle::create(View& view, bool colorBlending) {

    m_uboBuffer.create(
        UniformBufferCreateInfo().setSize(sizeof(RectangleUniform))
    );

    Shader vertexShader;
    Shader fragmentShader;

    vertexShader.setStage(VK_SHADER_STAGE_VERTEX_BIT).create(SHADER_PATH "rectangleC.vert.spv");
    fragmentShader.setStage(VK_SHADER_STAGE_FRAGMENT_BIT).create(SHADER_PATH "rectangleC.frag.spv");

    createDrawable(
        DrawableCreateInfo()
        .setColorBlendEnable(colorBlending)
        .setView(view)
        .setDepthTestEnable(false)
        .setMinSampleShading(0.0f)
        .setMultisamplingEnable(false)
        .setIstanceCount(1)
        // .addDescriptor(view.getCamera().getDescriptor())
        .addDescriptor(
            Descriptor().get(
                DescriptorInfo().addBuffersBinding(
                    BuffersBinding().addBuffer(m_uboBuffer).setStage(VK_SHADER_STAGE_VERTEX_BIT)
                )
            )
        )
        .loadIndicies(indicies)
        .loadVerticies(verticies)
        .addShader(vertexShader)
        .addShader(fragmentShader)
    );

    vertexShader.destroy();
    fragmentShader.destroy();
}

void Rectangle::create(View& view, Texture& texture, bool colorBlending) {

    m_uboBuffer.create(
        UniformBufferCreateInfo().setSize(sizeof(RectangleUniform))
    );

    Shader vertexShader;
    Shader fragmentShader;

    vertexShader.setStage(VK_SHADER_STAGE_VERTEX_BIT).create(SHADER_PATH "rectangleCT.vert.spv");
    fragmentShader.setStage(VK_SHADER_STAGE_FRAGMENT_BIT).create(SHADER_PATH "rectangleCT.frag.spv");

    createDrawable(
        DrawableCreateInfo()
        .setColorBlendEnable(colorBlending)
        .setView(view)
        .setDepthTestEnable(false)
        .setMinSampleShading(0.0f)
        .setMultisamplingEnable(false)
        .setIstanceCount(1)
        // .addDescriptor(view.getCamera().getDescriptor())
        .addDescriptor(
            Descriptor().get(
                DescriptorInfo()
                .addBuffersBinding(
                    BuffersBinding().addBuffer(m_uboBuffer).setStage(VK_SHADER_STAGE_VERTEX_BIT)
                )
                .addTexturesBinding(
                    TexturesBinding().addTexture(texture)
                )
            )
        )
        .loadIndicies(indicies)
        .loadVerticies(verticies)
        .addShader(vertexShader)
        .addShader(fragmentShader)
    );

    vertexShader.destroy();
    fragmentShader.destroy();
}

void Rectangle::destroy() {
    m_uboBuffer.destroy();
    freeDescriptor(m_setPools[1], m_descriptorSets[1]);
    m_vertex.buffer.destroy();
    m_index.buffer.destroy();
    vkDestroyPipeline(core::apiCore.device, m_pipeline, nullptr);
}

void Rectangle::uploadUniform(const RectangleUniform& uniform) {
    m_uboBuffer.load(&uniform, sizeof(uniform));
}

void Rectangle::setColor(const glm::vec4& color) {
    m_color = color;
}

void Rectangle::setColor(float r, float g, float b, float a) {
    m_color.r = r;
    m_color.g = g;
    m_color.b = b;
    m_color.a = a;
}

// FIXME
void Rectangle::upload() {
    RectangleUniform uniform;
    uniform.color = m_color;
    uniform.transform = getTransform();

    m_uboBuffer.load(&uniform, sizeof(uniform));
}

void RectangleFactory::addChild(RectangleInstance& instance) {
    if (m_instanceCount >= m_count) {
        throw std::runtime_error("RectangleFactory::addChild: attempt to fit " + std::to_string(m_instanceCount+1) +
        " children in a factory with size " + std::to_string(m_count));
    }

    instance.m_factoryOffset = m_instanceCount * sizeof(RectangleUniform);
    instance.m_uniform = &m_ubos[m_instanceCount];
    instance.m_uboBuffer = &m_uboBuffer;
    m_totalSize += sizeof(RectangleUniform);

    m_instanceCount++;
}

void RectangleFactory::create(View& view, uint32_t count, bool colorBlending) {
    m_count = count;
    m_ubos.resize(count);


    m_uboBuffer.create(
        UniformBufferCreateInfo().setSize(sizeof(RectangleUniform) * count)
    );

    Shader vertexShader;
    Shader fragmentShader;

    vertexShader
        .setStage(VK_SHADER_STAGE_VERTEX_BIT)
        .setSpecialization(
            ShaderSpecialization().add<uint32_t>(count)
        )
        .create(SHADER_PATH "factoryRectangleC.vert.spv");
    
    fragmentShader.setStage(VK_SHADER_STAGE_FRAGMENT_BIT).create(SHADER_PATH "rectangleC.frag.spv");

    createDrawable(
        DrawableCreateInfo()
        .setColorBlendEnable(colorBlending)
        .setView(view)
        .setDepthTestEnable(false)
        .setMinSampleShading(0.0f)
        .setMultisamplingEnable(false)
        .setIstanceCount(0)
        // .addDescriptor(view.getCamera().getDescriptor())
        .addDescriptor(
            Descriptor().get(
                DescriptorInfo().addBuffersBinding(
                    BuffersBinding().addBuffer(m_uboBuffer).setStage(VK_SHADER_STAGE_VERTEX_BIT)
                )
            )
        )
        .loadIndicies(indicies)
        .loadVerticies(verticies)
        .addShader(vertexShader)
        .addShader(fragmentShader)
    );

    vertexShader.destroy();
    fragmentShader.destroy();
}

void RectangleFactory::destroy() {
    m_uboBuffer.destroy();
    freeDescriptor(m_setPools[1], m_descriptorSets[1]);
    m_vertex.buffer.destroy();
    m_index.buffer.destroy();
    vkDestroyPipeline(core::apiCore.device, m_pipeline, nullptr);
}

void RectangleFactory::upload() {
    m_uboBuffer.load(m_ubos.data(), m_totalSize);
}

void TexturedRectangleFactory::addChild(TexturedRectangleInstance& instance) {
    if (m_instanceCount >= m_count) {
        throw std::runtime_error("RectangleFactory::addChild: attempt to fit " + std::to_string(m_instanceCount+1) +
        " children in a factory with size " + std::to_string(m_count));
    }

    instance.m_factoryOffset = m_instanceCount * sizeof(TexturedRectangleUniform);
    instance.m_uniform = &m_ubos[m_instanceCount];
    instance.m_uboBuffer = &m_uboBuffer;
    m_totalSize += sizeof(TexturedRectangleUniform);

    m_instanceCount++;
}

void TexturedRectangleFactory::create(View& view, uint32_t count, Texture& texture, bool colorBlending) {
    m_count = count;
    m_ubos.resize(count);

    DrawableCreateInfo createInfo;

    m_uboBuffer.create(
        UniformBufferCreateInfo().setSize(sizeof(TexturedRectangleUniform) * count)
    );

    Shader vertexShader;
    Shader fragmentShader;

    vertexShader
        .setStage(VK_SHADER_STAGE_VERTEX_BIT)
        .setSpecialization(
            ShaderSpecialization().add<uint32_t>(count)
        )
        .create(SHADER_PATH "factoryRectangleT.vert.spv");
    
    fragmentShader.setStage(VK_SHADER_STAGE_FRAGMENT_BIT).create(SHADER_PATH "rectangleT.frag.spv");

    createDrawable(
        DrawableCreateInfo()
        .setColorBlendEnable(colorBlending)
        .setView(view)
        .setDepthTestEnable(false)
        .setMinSampleShading(0.0f)
        .setMultisamplingEnable(false)
        .setIstanceCount(0)
        // .addDescriptor(view.getCamera().getDescriptor())
        .addDescriptor(
            Descriptor().get(
                DescriptorInfo()
                .addBuffersBinding(
                    BuffersBinding().addBuffer(m_uboBuffer).setStage(VK_SHADER_STAGE_VERTEX_BIT)
                )
                .addTexturesBinding(
                    TexturesBinding().addTexture(texture)
                )
            )
        )
        .loadIndicies(indicies)
        .loadVerticies(verticies)
        .addShader(vertexShader)
        .addShader(fragmentShader)
    );

    vertexShader.destroy();
    fragmentShader.destroy();
}

void TexturedRectangleFactory::destroy() {
    m_uboBuffer.destroy();
    freeDescriptor(m_setPools[1], m_descriptorSets[1]);
    m_vertex.buffer.destroy();
    m_index.buffer.destroy();
    vkDestroyPipeline(core::apiCore.device, m_pipeline, nullptr);
}

void TexturedRectangleFactory::upload() {
    m_uboBuffer.load(m_ubos.data(), m_totalSize);
}

void RectangleInstance::setColor(const glm::vec4& color) {
    m_uniform->color = color;
}

void RectangleInstance::setColor(float r, float g, float b, float a) {
    m_uniform->color.r = r;
    m_uniform->color.g = g;
    m_uniform->color.b = b;
    m_uniform->color.a = a;
}

void RectangleInstance::updateTransform() {
    m_uniform->transform = getTransform();
}

// FIXME
void RectangleInstance::upload() {
    m_uboBuffer->load(m_uniform, sizeof(RectangleUniform), m_factoryOffset);
}

void TexturedRectangleInstance::setTexCoords(glm::vec2 coords[4]) {
    for (size_t i = 0; i < 4; ++i) {
        m_uniform->texCoords[i] = coords[i];
    }
}

void TexturedRectangleInstance::updateTransform() {
    m_uniform->transform = getTransform();
}

// FIXME
void TexturedRectangleInstance::upload() {
    m_uboBuffer->load(m_uniform, sizeof(TexturedRectangleUniform), m_factoryOffset);
}

} // namespace age
