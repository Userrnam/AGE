#include "ActorFactory.hpp"

namespace age::actors {

    ActorFactory::ActorFactory() {
        // TODO(si-7): Here would go initialization of m_actorComponentCreators

        m_lastActorId = 0;
    }

    StrongActorPtr ActorFactory::createActor(const char* actorResource) {
        
    }

    StrongActorComponentPtr ActorFactory::createComponent(TiXmlElement* pData) {

    }

} // namespace age::actors