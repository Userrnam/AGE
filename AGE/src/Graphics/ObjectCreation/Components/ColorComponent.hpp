#pragma once

#include <sstream>

#include "../../../Math/Vector.hpp"
#include "ShaderComponent.hpp"

namespace age {

struct Color : public Vector4f{
    static ShaderComponentInfo __getInfo() {
        ShaderComponentInfo info;
        info.add(
            ShaderComponentBuffer()
            .addBlockMember("vec4 color", true)
            .setFragMainInsert("\tfragColor *= globals.color;\n")
        );
        return info;
    }

    inline Color(const Vector4f& v = Vector4f(1)) {
        x = v.x; y = v.y; z = v.z; w = v.w;
    }
    inline Color(float r, float g, float b, float a = 1.0f) { *this = Vector4f{r, g, b, a}; }

    inline const Color& get() const { return *this; }
    inline Color& get() { return *this; }

    inline void set(const Vector4f& data) { *this = data; }
};

inline Color operator*(float x, const Color& rhs) {
    return x * rhs.get();
}

} // namespace age
