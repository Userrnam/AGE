#pragma once

#include <glm/glm.hpp>

namespace age::core::window {

glm::ivec2 getWindowSize();
void create();
void destroy();
void pollEvents();
bool closed();
int present();

} // namespace age::core::window
