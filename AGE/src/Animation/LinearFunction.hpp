#pragma once

#include "AnimationState.hpp"

namespace age {

template <typename T>
bool linearFunction(AnimationState<T>* s0, AnimationState<T>* s1, float runTime, T* res) {
    if (s0->getRuntime() <= runTime) {
        if (s1 == nullptr) {
            return true;
        }
        *res = s1->getData();
        return true;
    }

    if (s1 == nullptr) {
        return false;
    }

    float completion = runTime / s0->getRuntime();

    *res = s0->getData() + completion * (s1->getData() - s0->getData());

    return false;
}

} // namespace age
