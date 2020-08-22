#pragma once

#include <sstream>
#include <glm/glm.hpp>
#include <vector>

#include "ShaderComponent.hpp"

#include "../Texture.hpp"

namespace age {

// ! requires vec2 texCoords in fragment shader
class TextureComponent {
    Texture m_texture;

public:
    void create(std::string path, Shared<Sampler> sampler)  {
        m_texture.create(path, sampler);
    }

    void destroy() {
        m_texture.destroy();
    }

    ShaderComponentInfo getInfo() {
        ShaderComponentInfo info;
        info.add(
            ShaderComponentTexture()
            .setName("textureSampler")
            .setTexture(m_texture)
        );
        info.setFragMainInsert("\tfragColor *= texture(textureSampler, globals.texCoords);\n");
        info.setId<TextureComponent>();
        return info;
    }
};

} // namespace age
