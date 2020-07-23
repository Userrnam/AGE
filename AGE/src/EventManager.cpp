#include <GLFW/glfw3.h>

#include "Graphics/Core/Core.hpp"

#include <iostream>

#include "EventManager.hpp"
#include "Arena.hpp"

namespace age {
extern Arena eventArena;
}

namespace age::core {
extern Core apiCore;
}

namespace age::EventManager {

std::vector<Event> m_events;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    event::Key eStruct;
    eStruct.key = key;
    eStruct.scancode = scancode;
    eStruct.action = action;
    eStruct.mods = mods;

    Event e;
    e.setId(hash("glfw_key"));
    e.setStructure<event::Key>(eStruct);
    sendEvent(e);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    event::Scroll eStruct;
    eStruct.xOffset = xOffset;
    eStruct.yOffset = yOffset;

    Event e;
    e.setId(hash("glfw_scroll"));
    e.setStructure<event::Scroll>(eStruct);
    sendEvent(e);
}

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos) {
    event::CursorPos eStruct;
    eStruct.xPos = xPos;
    eStruct.yPos = yPos;

    Event e;
    e.setId(hash("glfw_cursor_pos"));
    e.setStructure<event::CursorPos>(eStruct);
    sendEvent(e);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    event::MouseButton eStruct;
    eStruct.button = button;
    eStruct.action = action;
    eStruct.mods = mods;
    glfwGetCursorPos(core::apiCore.window.handle, &eStruct.xPos, &eStruct.yPos);

    Event e;
    e.setId(hash("glfw_mouse_button"));
    e.setStructure<event::MouseButton>(eStruct);
    sendEvent(e);
}

void init() {
    eventArena.init(16 * 1024);

    glfwSetKeyCallback(core::apiCore.window.handle, keyCallback);
    glfwSetScrollCallback(core::apiCore.window.handle, scrollCallback);
    glfwSetCursorPosCallback(core::apiCore.window.handle, cursorPosCallback);
    glfwSetMouseButtonCallback(core::apiCore.window.handle, mouseButtonCallback);
}

void destroy() {
    eventArena.destroy();
}

void sendEvent(Event& event) {
    m_events.push_back(event);
}

const std::vector<Event>& getEvents() {
    return m_events;
}

void clearEvents() {
    m_events.clear();
    eventArena.flush();
}

} // namespace age
