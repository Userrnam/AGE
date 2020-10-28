#include "Animator.hpp"

namespace age {

Animator::~Animator() {
    for (auto anim : m_runningAnimations) {
        delete anim.second;        
    }

    for (auto anim : m_pausedAnimations) {
        delete anim.second;
    }
}

void Animator::update(float elapsedTime) {
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
        // we know that data is located after buffer
        b->load(b + 1, b->getSize());
    }
}

void Animator::pauseAnimation(uint64_t id) {
    auto it = m_runningAnimations.find(id);
    if (it == m_runningAnimations.end()) {
        return;
    }
    m_pausedAnimations[id] = m_runningAnimations[id];
    m_runningAnimations.erase(id);
}

void Animator::continueAnimation(uint64_t id) {
    auto it = m_pausedAnimations.find(id);
    if (it == m_pausedAnimations.end()) {
        return;
    }
    m_runningAnimations[id] = m_pausedAnimations[id];
    m_pausedAnimations.erase(id);
}

void Animator::stopAnimation(uint64_t id) {
    auto anim = m_runningAnimations.find(id);

    if (anim == m_runningAnimations.end()) {
        return;
    }

    delete anim->second;
    m_runningAnimations.erase(anim);
}


} // namespace age
