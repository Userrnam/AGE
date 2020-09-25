#include "Animator.hpp"

namespace age {

uint64_t Animator::m_id = 0;
std::unordered_map<uint64_t, AnimationBase*> Animator::m_runningAnimations;
std::unordered_map<uint64_t, AnimationBase*> Animator::m_pausedAnimations;

} // namespace age
