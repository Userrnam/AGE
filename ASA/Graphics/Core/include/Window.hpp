#pragma once

namespace age::core::window {

void create(int width, int height, const char *title);
void pollEvents();
bool closed();
void present();

} // namespace age::core::window
