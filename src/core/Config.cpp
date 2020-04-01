#include "core/Config.hpp"
#include "core/AppProperties.hpp"

core::AppProperties appProperties;

namespace core {
namespace config {

void debugEnable(bool b) {
    appProperties.debugEnable = b;
}

} // namespace config
} // namespace core
