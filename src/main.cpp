#include <iostream>

#include "core/coreAPI.hpp"
#include "core/Window.hpp"


int main(int argc, char* argv[]) {
	core::debugEnable(true);
	core::init("hello", 0);

	core::window::create(100, 100, "hello");

	while (!core::window::closed()) {
		core::window::pollEvents();
	}

	core::destroy();

	return 0;
}
