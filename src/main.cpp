#include <iostream>

#include "core/AppProperties.hpp"


int main(int argc, char* argv[]) {
	core::AppProperties apP;
	apP.debugEnable(true);
	apP.createInstance(nullptr, 0);
	apP.destroy();

	return 0;
}
