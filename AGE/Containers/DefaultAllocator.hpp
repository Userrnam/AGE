#pragma once

#include <stdlib.h>

namespace age {

struct DefaultAllocator {
	static void* allocData(size_t size) {
		return malloc(size);
	}

	static void* reallocData(void* p, size_t size) {
		return realloc(p, size);
	}

	static void freeData(void *p) {
		free(p);
	}
};

} // namespace age
