#include <iostream>

#include "core/VulkanCore.hpp"
#include "core/Window.hpp"

int main(int argc, char* argv[]) {
	core::debugEnable(true);
	core::createInstance(nullptr, 0);

	core::Window window;
	window.init(100, 100, "hello");

	while(!window.closed()) {
		window.pollEvents();
	}

	core::destroy();

	return 0;
}
