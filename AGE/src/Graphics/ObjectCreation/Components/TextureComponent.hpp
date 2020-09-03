#pragma once

#include <sstream>
#include <glm/glm.hpp>
#include <vector>

#include "ShaderComponent.hpp"

#include "Objects/Texture.hpp"

namespace age {

// ! requires vec2 texCoords in fragment shader
class TextureComponent {
    Texture m_texture;

public:
    inline void setTexture(const Texture& texture) {
        m_texture = texture;
    }

    TextureComponent() {}
    inline TextureComponent(const Texture& texture) { m_texture = texture; }

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
