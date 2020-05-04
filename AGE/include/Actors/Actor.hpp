#pragma once

#include <inttypes.h>
#include <map>
#include <memory>

#include "ActorComponent.hpp"
#include "tinyxml.h"

namespace age::actors {

    class Actor;

    typedef std::shared_ptr<Actor> StrongActorPtr;
    typedef uint32_t ActorId;

    class Actor {
        friend class ActorFactory;

        typedef std::map<ComponentId, StrongActorComponentPtr> ActorComponents;

        ActorId m_id;
        ActorComponents m_components;

    public:
        explicit Actor(ActorId id) { m_id = id; }
        ~Actor();

        bool init(TiXmlElement* pData);
        void postInit();
        void destroy();
        void update();

        ActorId getId() const { return m_id; }

        template <class ComponentType>
        std::weak_ptr<ComponentType> getComponent(ComponentId id);

    private:
        void addComponent(StrongActorComponentPtr pComponent);
    };

} // namespace age::actors