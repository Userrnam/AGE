#pragma once

#include <vector>

#define TRANSITION_HANDLER(name) \
    void name(const age::StatePointer __pStartState, const age::StatePointer __pEndState, age::StatePointer __pCurrentState, float alpha)

#define TRANSITION_SET_STATE_TYPE(type) \
    type* pStartState = (type*)__pStartState; \
    type* pEndState = (type*)__pEndState; \
    type* pCurrentState = (type*)__pCurrentState

#define START_STATE (*pStartState)
#define END_STATE (*pEndState)
#define CURRENT_STATE (*pStartState)

namespace age {

enum AnimationState {
    ANIMATION_STATE_RUNNING,
    ANIMATION_STATE_COMPLETED,
};

typedef void* StatePointer;
typedef void(*TransitionHandler)(const StatePointer pStartState, const StatePointer pEndState, StatePointer pCurrentState, float alpha);

class Animation {
    float m_delta; // how much animation has run
    float m_duration;
    StatePointer m_pStartState;
    StatePointer m_pEndState;
    StatePointer m_pCurrentState;
    TransitionHandler m_transition;

public:
    Animation(float duration, StatePointer pStartState, StatePointer pEndState, TransitionHandler transition) 
        : m_duration(duration), m_pStartState(pStartState), m_pEndState(pEndState), m_transition(transition) {}

    inline void restart();
    inline AnimationState update(float delta);
    inline StatePointer getCurrentStatePointer();
};

void Animation::restart() {
    m_delta = 0;
}

AnimationState Animation::update(float delta) {
    m_delta += delta;
    
    if (m_delta >= m_duration) {
        m_transition(m_pStartState, m_pEndState, m_pCurrentState, 1.0f);
        return ANIMATION_STATE_COMPLETED;
    }

    m_transition(m_pStartState, m_pEndState, m_pCurrentState, m_delta / m_duration);

    return ANIMATION_STATE_RUNNING;
}

StatePointer Animation::getCurrentStatePointer() {
    return m_pCurrentState;
}

} // namespace age
