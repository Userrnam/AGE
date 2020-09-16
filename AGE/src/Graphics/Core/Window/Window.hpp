#pragma once

#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>

namespace age::core::window {

glm::ivec2 getWindowSize();
void create();
void pollEvents();
bool closed();
void present();

} // namespace age::core::window
