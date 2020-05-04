#pragma once

#include "Actor.hpp"
#include "ActorComponent.hpp"

namespace age::actors {

    typedef ActorComponent *(*ActorComponentCreator)(void);
    typedef std::map<std::string, ActorComponentCreator> ActorComponentCreatorMap;

    class ActorFactory {
        ActorId m_lastActorId;
        ActorComponentCreatorMap m_actorComponentCreators;
    
    public:
        ActorFactory();
        StrongActorPtr createActor(const char* actorResource);

    private:
        virtual StrongActorComponentPtr createComponent(TiXmlElement* pData);
        ActorId getNextActorId() { ++m_lastActorId; return m_lastActorId; }
    };

} // namespace age::actors