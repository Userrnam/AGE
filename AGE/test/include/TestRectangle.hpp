#pragma once

#include "Graphics.hpp"
#include "Graphics/ShapeManager.hpp"
#include "Graphics/Components/ColorComponent.hpp"
#include "Graphics/Components/TransformComponent.hpp"
#include "Graphics/ShaderBuilder.hpp"

#include <glm/gtc/matrix_transform.hpp>

struct TestRectangle : public age::Drawable {
    age::ColorComponent color;
    age::TransformComponent transform;

    void create(age::View& view) {
        color.create();
        color.set({1, 1, 0, 1});

        transform.create();
        auto& t = transform.get();
        transform.set(glm::scale(glm::mat4(1), glm::vec3(1000, 1000, 1)));

        age::ShaderBuilder builder;
        auto vs = builder.compileVertexShader(color, transform);
        auto fs = builder.compileFragmentShader(color, transform);
        Drawable::create(
            age::DrawableCreateInfo()
            .setColorBlendEnable(false)
            .setIstanceCount(1)
            .setShapeId(age::RECTANGLE_SHAPE)
            .addDescriptorSet(
                age::DescriptorSet()
                .get(
                    age::DescriptorSetInfo()
                    .getBasedOnComponents(color, transform)
                )
            )
            .addShader(vs)
            .addShader(fs)
        );

        vs.destroy();
        fs.destroy();
    }

    void destroy() {
        age::destroyPipeline(m_pipeline);
        age::freeDescriptor(m_poolIndicies[1], m_descriptorSets[1]);
        color.destroy();
        transform.destroy();
    }
};
