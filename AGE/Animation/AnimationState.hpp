#pragma once

#include <stdlib.h>

namespace age {

template<typename T>
class AnimationState {
    T m_data;
    float m_runtime;

public:
    AnimationState(T data, float runtime = 0) : m_data(data), m_runtime(runtime) {}

    inline T& getData() { return m_data; }
    inline float getRuntime() { return m_runtime; }
};

} // namespace age
