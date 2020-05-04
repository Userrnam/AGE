#pragma once

#include <memory>
#include <inttypes.h>

namespace age::actors {

    class ActorComponent;

    typedef uint32_t ComponentId;
    typedef std::shared_ptr<ActorComponent> StrongActorComponentPtr;

    class ActorComponent {
        friend class ActorFactory;

    protected:
        StrongActorPtr m_pOwner;

    public:
        virtual ~ActorComponent();

        virtual bool vInit(TiXmlElement* pData) = 0;
        virtual void vPostInit() {}
        virtual void vUpdate() {}

        virtual ComponentId vGetComponentId() const = 0;

    private:
        void setOwner(StrongActorPtr pOwner) { m_pOwner = pOwner; }
    };

} // namespace age::actors