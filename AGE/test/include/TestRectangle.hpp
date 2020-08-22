#pragma once

#include "Graphics.hpp"
#include "Graphics/BasicShape.hpp"
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
        color.upload();

        transform.create();
        auto& t = transform.get();
        transform.set(glm::scale(glm::mat4(1), glm::vec3(1000, 1000, 1)));
        transform.upload();

        auto shape = age::getRectangleShape();

        age::ShaderBuilder builder;
        auto vs = builder.compileVertexShader(color, transform);
        auto fs = builder.compileFragmentShader(color, transform);
        createDrawable(
            age::DrawableCreateInfo()
            .setColorBlendEnable(false)
            .setIstanceCount(1)
            .setView(view)
            .setShapeInfo(shape)
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