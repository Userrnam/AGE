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
    { { 0.0, 0.0 } },
    { { 1.0, 0.0 } },
    { { 1.0, 1.0 } },
    { { 0.0, 1.0 } },
};

VERTEX_ATTRIBUTES(VType) = {
    FORMAT_R32G32_SFLOAT
};

static std::vector<Index16> indicies = { 0, 1, 2, 2, 3, 0 };

void Rectangle::preCreate(View& view, ObjectCreateInfo& createInfo) {
    m_isOwner = true;

    core::BufferCreateInfo bufferInfo = {};
    bufferInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    bufferInfo.size = sizeof(RectangleUniform);
    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    m_uboBuffer.create(bufferInfo);

    createInfo.depthTest = false;

    createInfo.descriptors.push_back(view.getCamera().getDescriptor());

    createInfo.index.buffer = core::createDeviceLocalBuffer(indicies.data(), indicies.size() * sizeof(indicies[0]), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    createInfo.index.count = indicies.size();
    createInfo.index.type = VK_INDEX_TYPE_UINT16;
    createInfo.minSampleShading = 0.0f;
    Vertex<VType>::fillAttributes(&createInfo.vertex.attributeDescriptions);
    Vertex<VType>::fillBinding(&createInfo.vertex.bindingDescription);
    createInfo.vertex.buffer = core::createDeviceLocalBuffer(verticies.data(), verticies.size() * sizeof(VType), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    createInfo.viewport = view.getViewport();
}

void Rectangle::create(const Rectangle& sample) {
    m_isOwner = false;

    m_renderPass = sample.m_renderPass;
    m_vertex = sample.m_vertex;
    m_index = sample.m_index;
    m_pipeline = sample.m_pipeline;
    m_pipelineLayout = sample.m_pipelineLayout;
    m_descriptorSets = sample.m_descriptorSets;
    m_instanceCount = sample.m_instanceCount;

    core::BufferCreateInfo bufferInfo = {};
    bufferInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    bufferInfo.size = sizeof(RectangleUniform);
    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    m_uboBuffer.create(bufferInfo);

    // replace ubo descriptor
    m_descriptorSets[1] = Descriptor().get(
        DescriptorInfo().addBuffer(m_uboBuffer)
    ).getSet();
}

void Rectangle::create(const Rectangle& sample, Texture& texture) {
    m_isOwner = false;

    m_renderPass = sample.m_renderPass;
    m_vertex = sample.m_vertex;
    m_index = sample.m_index;
    m_pipeline = sample.m_pipeline;
    m_pipelineLayout = sample.m_pipelineLayout;
    m_descriptorSets = sample.m_descriptorSets;

    core::BufferCreateInfo bufferInfo = {};
    bufferInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    bufferInfo.size = sizeof(RectangleUniform);
    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    m_uboBuffer.create(bufferInfo);

    // replace ubo descriptor
    m_descriptorSets[1] = Descriptor().get(
        DescriptorInfo().addBuffer(m_uboBuffer).addTexture(texture)
    ).getSet();
}

void Rectangle::create(View& view, bool colorBlending) {
    ObjectCreateInfo createInfo;
    createInfo.colorBlending = colorBlending;
    preCreate(view, createInfo);

    createInfo.descriptors.push_back(
        Descriptor().get(
            DescriptorInfo().addBuffer(m_uboBuffer)
        )
    );

    createInfo.shaders.push_back(
        Shader().setStage(VK_SHADER_STAGE_VERTEX_BIT).create(SHADER_PATH "rectangleC.vert.spv")
    );
    createInfo.shaders.push_back(
        Shader().setStage(VK_SHADER_STAGE_FRAGMENT_BIT).create(SHADER_PATH "rectangleC.frag.spv")
    );

    createObject(createInfo);

    for (auto& shader : createInfo.shaders) {
        shader.destroy();
    }
}

void Rectangle::create(View& view, Texture& texture, bool colorBlending) {
    ObjectCreateInfo createInfo;
    createInfo.colorBlending = colorBlending;
    preCreate(view, createInfo);

    createInfo.descriptors.push_back(
        Descriptor().get(
            DescriptorInfo().addBuffer(m_uboBuffer).addTexture(texture)
        )
    );

    createInfo.shaders.push_back(
        Shader().setStage(VK_SHADER_STAGE_VERTEX_BIT).create(SHADER_PATH "rectangleCT.vert.spv")
    );
    createInfo.shaders.push_back(
        Shader().setStage(VK_SHADER_STAGE_FRAGMENT_BIT).create(SHADER_PATH "rectangleCT.frag.spv")
    );

    createObject(createInfo);

    for (auto& shader : createInfo.shaders) {
        shader.destroy();
    }
}

void Rectangle::destroy() {
    m_uboBuffer.destroy();
    freeDescriptor(m_setPools[1], m_descriptorSets[1]);
    if (m_isOwner) {
        m_vertex.buffer.destroy();
        m_index.buffer.destroy();
        vkDestroyPipeline(core::apiCore.device, m_pipeline, nullptr);
    }
}

void Rectangle::uploadUniform(const RectangleUniform& uniform) {
    m_uboBuffer.loadData(&uniform, sizeof(uniform));
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

    m_uboBuffer.loadData(&uniform, sizeof(uniform));
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

    ObjectCreateInfo createInfo;
    createInfo.colorBlending = colorBlending;

    core::BufferCreateInfo bufferInfo = {};
    bufferInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    bufferInfo.size = sizeof(RectangleUniform) * count;
    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    m_uboBuffer.create(bufferInfo);

    createInfo.depthTest = false;
    createInfo.descriptors.push_back(view.getCamera().getDescriptor());

    createInfo.index.buffer = core::createDeviceLocalBuffer(indicies.data(), indicies.size() * sizeof(indicies[0]), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    createInfo.index.count = indicies.size();
    createInfo.index.type = VK_INDEX_TYPE_UINT16;
    createInfo.minSampleShading = 0.0f;
    Vertex<VType>::fillBinding(&createInfo.vertex.bindingDescription);
    Vertex<VType>::fillAttributes(&createInfo.vertex.attributeDescriptions);
    createInfo.vertex.buffer = core::createDeviceLocalBuffer(verticies.data(), verticies.size() * sizeof(VType), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    createInfo.viewport = view.getViewport();
    createInfo.instanceCount = 0;

    createInfo.descriptors.push_back(
        Descriptor().get(
            DescriptorInfo().addBuffer(m_uboBuffer)
        )
    );

    createInfo.shaders.push_back(
        Shader()
            .setStage(VK_SHADER_STAGE_VERTEX_BIT)
            .setSpecialization(
                ShaderSpecialization().add<uint32_t>(count)
            )
            .create(SHADER_PATH "factoryRectangleC.vert.spv")
    );
    createInfo.shaders.push_back(
        Shader().setStage(VK_SHADER_STAGE_FRAGMENT_BIT).create(SHADER_PATH "rectangleC.frag.spv")
    );

    createObject(createInfo);

    for (auto& shader : createInfo.shaders) {
        shader.destroy();
    }
}

void RectangleFactory::destroy() {
    m_uboBuffer.destroy();
    freeDescriptor(m_setPools[1], m_descriptorSets[1]);
    m_vertex.buffer.destroy();
    m_index.buffer.destroy();
    vkDestroyPipeline(core::apiCore.device, m_pipeline, nullptr);
}

void RectangleFactory::upload() {
    m_uboBuffer.loadData(m_ubos.data(), m_totalSize);
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

    ObjectCreateInfo createInfo;
    createInfo.colorBlending = colorBlending;

    core::BufferCreateInfo bufferInfo = {};
    bufferInfo.memoryProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    bufferInfo.size = sizeof(TexturedRectangleUniform) * count;
    bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    m_uboBuffer.create(bufferInfo);

    createInfo.depthTest = false;
    createInfo.descriptors.push_back(view.getCamera().getDescriptor());

    createInfo.index.buffer = core::createDeviceLocalBuffer(indicies.data(), indicies.size() * sizeof(indicies[0]), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    createInfo.index.count = indicies.size();
    createInfo.index.type = VK_INDEX_TYPE_UINT16;
    createInfo.minSampleShading = 0.0f;
    Vertex<VType>::fillBinding(&createInfo.vertex.bindingDescription);
    Vertex<VType>::fillAttributes(&createInfo.vertex.attributeDescriptions);
    createInfo.vertex.buffer = core::createDeviceLocalBuffer(verticies.data(), verticies.size() * sizeof(VType), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    createInfo.viewport = view.getViewport();
    createInfo.instanceCount = 0;

    createInfo.descriptors.push_back(
        Descriptor().get(
            DescriptorInfo().addBuffer(m_uboBuffer).addTexture(texture)
        )
    );
    
    createInfo.shaders.push_back(
        Shader().setStage(VK_SHADER_STAGE_VERTEX_BIT).create(SHADER_PATH "factoryRectangleT.vert.spv")
    );
    createInfo.shaders.push_back(
        Shader().setStage(VK_SHADER_STAGE_FRAGMENT_BIT).create(SHADER_PATH "rectangleT.frag.spv")
    );

    createObject(createInfo);

    for (auto& shader : createInfo.shaders) {
        shader.destroy();
    }
}

void TexturedRectangleFactory::destroy() {
    m_uboBuffer.destroy();
    freeDescriptor(m_setPools[1], m_descriptorSets[1]);
    m_vertex.buffer.destroy();
    m_index.buffer.destroy();
    vkDestroyPipeline(core::apiCore.device, m_pipeline, nullptr);
}

void TexturedRectangleFactory::upload() {
    m_uboBuffer.loadData(m_ubos.data(), m_totalSize);
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
    m_uboBuffer->loadData(m_uniform, sizeof(RectangleUniform), m_factoryOffset);
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
    m_uboBuffer->loadData(m_uniform, sizeof(TexturedRectangleUniform), m_factoryOffset);
}

} // namespace age
