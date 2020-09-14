#include <GLFW/glfw3.h>
#include <iostream>

#include "Graphics/Core/Core.hpp"

#include "EventManager.hpp"
#include "Containers.hpp"

namespace age {
extern Arena eventArena;
}

namespace age::core {
extern Core apiCore;
}

namespace age::EventManager {

std::vector<Event> m_events;
bool pressedKeys[GLFW_KEY_LAST + 1] = {};

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    event::Key eStruct;
    eStruct.key = key;
    eStruct.scancode = scancode;
    eStruct.action = action;
    eStruct.mods = mods;

    // glfw release is 0, glfw press is 1
    pressedKeys[key] = action;

    Event e;
    e.setId(hash("glfw_key"));
    e.setStructure(eStruct);
    sendEvent(e);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    event::Scroll eStruct;
    eStruct.xOffset = xOffset;
    eStruct.yOffset = yOffset;

    Event e;
    e.setId(hash("glfw_scroll"));
    e.setStructure(eStruct);
    sendEvent(e);
}

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos) {
    event::CursorPos eStruct;
    eStruct.xPos = xPos;
    eStruct.yPos = yPos;

    Event e;
    e.setId(hash("glfw_cursor_pos"));
    e.setStructure(eStruct);
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
    e.setStructure(eStruct);
    sendEvent(e);
}

void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    core::apiCore.framebufferResized = true;

    event::Resize eStruct;
    eStruct.width = width;
    eStruct.height = height;

    Event e;
    e.setId(hash("glfw_resize"));
    e.setStructure(eStruct);
    sendEvent(e);
}

void init() {
    eventArena.init(16 * 1024);

    glfwSetKeyCallback(core::apiCore.window.handle, keyCallback);
    glfwSetScrollCallback(core::apiCore.window.handle, scrollCallback);
    glfwSetCursorPosCallback(core::apiCore.window.handle, cursorPosCallback);
    glfwSetMouseButtonCallback(core::apiCore.window.handle, mouseButtonCallback);
    glfwSetFramebufferSizeCallback(core::apiCore.window.handle, framebufferResizeCallback);
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

} // namespace age::EventManager

namespace age {

bool isKeyPressed(KeyCode keyCode) {
    return age::EventManager::pressedKeys[keyCode];
}

} // namespace age
