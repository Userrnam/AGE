#pragma once

#include "ShapeInfo.hpp"
#include "Containers/Shared.hpp"

namespace age {

// todo: manage shapes somehow differently

void initBasicShapes();
void destroyBasicShapes();

Shared<ShapeInfo> getRectangleShape();

} // namespace age
