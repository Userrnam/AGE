#pragma once

#include <unordered_map>

#include "Animation.hpp"

#include <iostream>

namespace age {

class Animator {
    static uint64_t m_id;
    static std::unordered_map<uint64_t, AnimationBase*> m_runningAnimations;
    static std::unordered_map<uint64_t, AnimationBase*> m_pausedAnimations;
public:
    static inline void update(float elapsedTime) {
        for (auto& anim : m_runningAnimations) {
            if (anim.second->update(elapsedTime)) {
                delete anim.second;
                m_runningAnimations.erase(anim.first);
            }
        }
    }

    static inline void pauseAnimation(uint64_t id) {
        auto it = m_runningAnimations.find(id);
        if (it == m_runningAnimations.end()) {
            return;
        }
        m_pausedAnimations[id] = m_runningAnimations[id];
        m_runningAnimations.erase(id);
    }

    static inline void continueAnimation(uint64_t id) {
        auto it = m_pausedAnimations.find(id);
        if (it == m_pausedAnimations.end()) {
            return;
        }
        m_runningAnimations[id] = m_pausedAnimations[id];
        m_pausedAnimations.erase(id);
    }

    // removes animation
    // Note: animation must be running
    static inline void stopAnimation(uint64_t id) {
        m_runningAnimations.erase(id);
    }

    // returns animation id
    static inline uint64_t addAnimation(AnimationBase* animation) {
        m_id++;

        animation->m_id = m_id;
        m_runningAnimations[m_id] = animation;
        return m_id;
    }
};

} // namespace age
