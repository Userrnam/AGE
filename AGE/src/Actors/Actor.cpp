#include "Actor.hpp"

namespace age::actors {

    Actor::~Actor() {}

    bool Actor::init(TiXmlElement* pData) {}

    void Actor::postInit() {}

    void Actor::destroy() {}

    void Actor::update() {}

    ActorId Actor::getId() const {}

    template <class ComponentType>
    std::weak_ptr<ComponentType> Actor::getComponent(ComponentId id) {
        ActorComponents::iterator findIt = m_components.find(id);
        if (findIt != m_components.end()) {
            StrongActorComponentPtr pBase(findIt->second);
            std::shared_ptr<ComponentType> pSub(std::static_pointer_cast<ComponentType>(pBase));
            std::weak_ptr<ComponentType> pWeakSub(pSub);

            return pWeakSub;
        }
        else {
            return std::weak_ptr<ComponentType>();
        }
    }

    void Actor::addComponent(StrongActorComponentPtr pComponent) {}

}