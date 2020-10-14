#pragma once

#include <unordered_map>
#include <unordered_set>

#include "Animation.hpp"

#include <iostream>

namespace age {

class Animator {
    static uint64_t m_id;
    static std::unordered_map<uint64_t, AnimationBase*> m_runningAnimations;
    static std::unordered_map<uint64_t, AnimationBase*> m_pausedAnimations;
public:
    static inline void update(float elapsedTime) {
        std::unordered_set<Buffer*> animationUploadBuffers;
        std::unordered_set<IResolveStructure*> resolveStructures;
        std::vector<uint64_t> animationsToRemove;

        for (auto& anim : m_runningAnimations) {
            if (anim.second->m_buffer) {
                animationUploadBuffers.emplace(anim.second->m_buffer);
            }
            if (anim.second->m_resolveStructure) {
                resolveStructures.emplace(anim.second->m_resolveStructure);
            }
            if (anim.second->update(elapsedTime)) {
                animationsToRemove.push_back(anim.first);
            }
        }
        
        for (auto a : animationsToRemove) {
            delete m_runningAnimations[a];
            m_runningAnimations.erase(a);
        }
        
        // update resolve structures
        for (auto r : resolveStructures) {
            r->resolve();
        }

        // upload changes
        for (auto b : animationUploadBuffers) {
            // ! Unsafe (we know that data is located after buffer)
            b->load(b + 1, b->getSize());
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
        auto anim = m_runningAnimations.find(id);

        if (anim == m_runningAnimations.end()) {
            return;
        }

        delete anim->second;
        m_runningAnimations.erase(anim);
    }

    // returns animation id
    template<typename AnimationType>
    static inline uint64_t addAnimation(const AnimationType& animation) {
        m_id++;

        auto pAnimation = new AnimationType(animation);
        pAnimation->m_id = m_id;
        m_runningAnimations[m_id] = pAnimation;
        return m_id;
    }
};

} // namespace age
