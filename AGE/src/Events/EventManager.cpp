#include "EventManager.hpp"

namespace age::events {

    static IEventManager* g_pEventMgr = NULL;

    IEventManager::IEventManager(const char* pname, bool setAsGlobal) {
        if (setAsGlobal) {
            if (g_pEventMgr) {
                // TODO(si-7): Warning system to warn about global event manager changing
                delete g_pEventMgr;
            }

            g_pEventMgr = this;
        }
    }

    IEventManager::~IEventManager() {
        if (g_pEventMgr == this) {
            g_pEventMgr = NULL;
        }
    }

    IEventManager* IEventManager::get() {
        // TODO(si-7): Warn on no global event manager set
        return g_pEventMgr;
    }

} // namespace age::events