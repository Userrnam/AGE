#pragma once

#include <AGE/Graphics.hpp>
#include <AGE/Scene.hpp>


struct ShaderEffect {
    static age::ShaderComponentInfo getInfo() {
        age::ShaderComponentInfo info;
        info.add(
            age::ShaderComponentForward("")
            .setFragMainInsert(
            "\tvec2 resolution = globals.resolution;\n"
            "\tvec2 coord = 20.0 * (gl_FragCoord.xy - resolution) / min(resolution.x, resolution.y);\n"
            "\tfloat len;\n"
            "\tfor (int i = 0; i < 12; ++i) {\n"
            "\t   len = length(vec2(coord.x, coord.y));\n"
            "\t   coord.x = coord.x - cos(coord.y + sin(len)) + cos(globals.time.x / 9.0);\n"
            "\t   coord.y = coord.y - cos(coord.x + sin(len)) + cos(globals.time.x / 12.0);\n"
            "\t}\n"
            "\tfragColor = vec4(cos(len * 2.0), cos(len * 3.0), cos(len * 1.0), 1.0);\n"
            )
        );
        info.setId<ShaderEffect>();
        return info;
    }
};

std::vector<age::Vector2f> verticies = {
    { 800, 0 },
    { 800, 600 },
    { 0, 600 }
};

std::vector<age::Index16> indicies = {
    0, 1, 2
};

struct Triangle : public age::StaticScriptComponent {
    age::Transformable transformable;
    age::StorageComponent<age::Transform> transform;
    age::ShapeId shapeId;

    Triangle(Entity e) : age::StaticScriptComponent(e) {
        transformable.create(e, age::Vector2f(800, 600));

        transform.create(transformable.getTransform());

        shapeId = age::Shape::create(
            age::ShapeCreateInfo().loadIndicies(indicies).loadVerticies(verticies)
        );

        addComponent<age::Drawable>(shapeId,
            transform,
            ShaderEffect()
        );
    }

    ~Triangle() {
        transformable.destroy();
        transform.destroy();

        getComponent<age::Drawable>().destroy();
    }
};
