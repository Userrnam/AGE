#pragma once

#include <unordered_map>
#include <unordered_set>

#include "Animation.hpp"
#include "../Events/Event.hpp"
#include "../Events/EventManager.hpp"

namespace age {

class Animator {
    EVENT_CALLBACK(Animator, update);

    uint64_t m_id = 0;
    std::unordered_map<uint64_t, AnimationBase*> m_runningAnimations;
    std::unordered_map<uint64_t, AnimationBase*> m_pausedAnimations;
public:
    Animator() {}
    ~Animator();

    Animator(const Animator&) = delete;

    void update(const event::Update& e);

    void pauseAnimation(uint64_t id);
    void continueAnimation(uint64_t id);

    // removes animation
    // Note: animation must be running
    void stopAnimation(uint64_t id);

    // returns animation id
    template<typename AnimationType>
    uint64_t addAnimation(const AnimationType& animation) {
        m_id++;

        auto pAnimation = new AnimationType(animation);
        pAnimation->m_id = m_id;
        m_runningAnimations[m_id] = pAnimation;
        return m_id;
    }
};

} // namespace age
