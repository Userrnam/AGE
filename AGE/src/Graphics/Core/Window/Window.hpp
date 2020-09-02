#pragma once

namespace age::core::window {

void create();
void pollEvents();
bool closed();
void render();
void present();

} // namespace age::core::window
