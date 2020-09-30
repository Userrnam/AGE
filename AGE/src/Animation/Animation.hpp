#pragma once

#include <vector>
#include <stdint.h>
#include <stdint.h>

#include "State.hpp"

namespace age {

class AnimationBase {
    uint64_t m_id;
    friend class Animator;

public:
    // return true if animation completed
    virtual bool update(float elapsedTime) = 0;
    virtual ~AnimationBase() {}
};

template <typename T>
using TransitionFunction = bool(AnimationState<T>* s0, AnimationState<T>* s1, float runTime, T* res);

template<typename T, TransitionFunction<T> tf>
class StateAnimation : public AnimationBase {
    // this can be pointer to user struct
    T* m_currentState = 0;
    
    std::vector<AnimationState<T>> m_states;
    uint32_t m_currentIndex = 0;
    float m_runTime = 0;
    bool m_looping = false;
    // ???
    int dummy;
public:
    StateAnimation() {}

    StateAnimation(T* pData) {
        m_currentState = pData;
    }

    inline StateAnimation& setDataPointer(T* pData) {
        m_currentState = pData;
        return *this;
    }

    inline StateAnimation& setLooping(bool flag) {
        m_looping = flag;
        return *this;
    }

    inline StateAnimation& addState(const AnimationState<T>& state) {
        m_states.push_back(state);
        return *this;
    }

    virtual bool update(float elapsedTime) override {
        bool last = m_currentIndex == m_states.size() - 1;
        auto pNext = last ? nullptr : &m_states[m_currentIndex + 1];
        if (!pNext && m_looping) {
            pNext = &m_states[0];
        }

        if (tf(&(m_states[m_currentIndex]), pNext, m_runTime, m_currentState)) {
            if (last) {
                if (!m_looping) {
                    return true;
                } else {
                    m_currentIndex = 0;
                }
            } else {
                m_currentIndex++;
            }

            m_runTime = 0;

            return false;
        }

        m_runTime += elapsedTime;

        return false;
    }
};

} // namespace age
