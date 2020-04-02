#include <iostream>

#include "core/coreAPI.hpp"
#include "core/Window.hpp"


int main(int argc, char* argv[]) {
	core::debugEnable(true);
	core::createInstance(nullptr, 0);

	core::window::create(100, 100, "hello");

	core::destroy();

	return 0;
}
