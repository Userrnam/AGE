#pragma once

#include "AnimationState.hpp"

namespace age {

template <typename T>
bool stepFunction(AnimationState<T>* s0, AnimationState<T>* s1, float runTime, T* res) {
    if (s0->getRuntime() <= runTime) {
        if (s1 == nullptr) {
            return true;
        }
        *res = s1->getData();
        return true;
    }
    return false;
}

} // namespace age
