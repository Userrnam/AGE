#include <GLFW/glfw3.h>
#include <unordered_map>
#include <vector>
#include <string.h>

#include "../Graphics/Core/Core.hpp"

#include "EventManager.hpp"
#include "../Containers/Arena.hpp"
#include "../Utils/Logger.hpp"

namespace age::core {
extern Core apiCore;
}

namespace age {

namespace detail {

void Event::setStructure(void* p, size_t size) {
    m_structure = Arena::allocate(size);
    memcpy(m_structure, p, size);
}

}

std::unordered_map<size_t, std::vector<detail::CallbackStructure>> EventManager::m_callbacks;

// Events buffer
std::vector<detail::Event> EventManager::m_events;

// input data
age::Vector2i m_previousWindowSize;
bool m_pressedKeys[GLFW_KEY_LAST+1] = {};

// callbacks

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    event::Key eStruct;
    eStruct.key = key;
    eStruct.scancode = scancode;
    eStruct.action = action;
    eStruct.mods = mods;

    m_pressedKeys[key] = action;

    EventManager::sendEvent(eStruct);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    event::Scroll eStruct;
    eStruct.offset.x = xOffset;
    eStruct.offset.y = yOffset;

    EventManager::sendEvent(eStruct);
}

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos) {
    event::CursorPos eStruct;
    eStruct.pos.x = xPos;
    eStruct.pos.y = core::apiCore.window.height - yPos;

    EventManager::sendEvent(eStruct);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    double x, y;
    glfwGetCursorPos(core::apiCore.window.handle, &x, &y);

    event::MouseButton eStruct;
    eStruct.button = button;
    eStruct.action = action;
    eStruct.mods = mods;
    eStruct.pos.x = x;
    eStruct.pos.y = core::apiCore.window.height - y;

    EventManager::sendEvent(eStruct);
}

void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    core::apiCore.framebufferResized = true;

    event::Resize eStruct;
    eStruct.oldSize = m_previousWindowSize;
    glfwGetWindowSize(window, &eStruct.newSize.x, &eStruct.newSize.y);

    EventManager::sendEvent(eStruct);
}

// end callbacks

void EventManager::__init() {
    glfwSetKeyCallback(core::apiCore.window.handle, keyCallback);
    glfwSetScrollCallback(core::apiCore.window.handle, scrollCallback);
    glfwSetCursorPosCallback(core::apiCore.window.handle, cursorPosCallback);
    glfwSetMouseButtonCallback(core::apiCore.window.handle, mouseButtonCallback);
    glfwSetFramebufferSizeCallback(core::apiCore.window.handle, framebufferResizeCallback);

    m_previousWindowSize.x = core::apiCore.window.width;
    m_previousWindowSize.y = core::apiCore.window.height;
}

void EventManager::__destroy() {
    for (auto& fs : m_callbacks) {
        for (auto& cbs : fs.second) {
            Logger::error("callback with handler %x for caller %x has not been freed", cbs.eventHandler, cbs.caller);
        }
    }
}

void EventManager::__processEvents() {
    for (auto& event : m_events) {
        auto& fs = m_callbacks[event];
        for (auto& cbs : fs) {
            cbs.eventHandler(cbs.caller, event.getStructurePointer());
        }
    }
    m_events.clear();
}

void EventManager::__sendEvent(const detail::Event& event) {
    m_events.push_back(event);
}

void EventManager::__registerCallback(size_t eid, void *caller, detail::EventHandler handler) {
    detail::CallbackStructure cbs;
    cbs.caller = caller;
    cbs.eventHandler = handler;
    m_callbacks[eid].push_back(cbs);
}

void EventManager::__forgetCallback(void *caller, size_t eid) {
    auto& fs = m_callbacks[eid];
    for (size_t i = 0; i < fs.size(); ++i) {
        if (fs[i].caller == caller) {
            fs.erase(fs.begin()+i);
            return;
        }
    }

    Logger::error("attempt to free unknown callback. caller: %x, eid: %x", caller, eid);
}

} // namespace age

namespace age {

bool isKeyPressed(KeyCode keyCode) {
    return age::m_pressedKeys[keyCode];
}

Vector2f getCursorPosition() {
    double x, y;
    glfwGetCursorPos(core::apiCore.window.handle, &x, &y);

    Vector2f pos;
    pos.x = x;
    pos.y = core::apiCore.window.height - y;

    return pos;
}

} // namespace age
