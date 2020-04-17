#pragma once

namespace core {
namespace window {

void create(int width, int height, const char *title);
void pollEvents();
bool closed();
void present();

} // namespace window
} // namespace core
