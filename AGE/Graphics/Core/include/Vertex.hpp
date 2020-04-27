#include <glm/glm.hpp>
#include <vector>

#include "Format.hpp"


namespace age::core {

struct BindingDescriptoion {
    uint32_t binding;
    uint32_t stride;
};

struct AttributeDescription {
    uint32_t binding;
    uint32_t location;
    uint32_t format;
    uint32_t offset;
};

struct Vertex {
    virtual BindingDescriptoion getBindingDescription() = 0;
    virtual std::vector<AttributeDescription> getAttributeDescriptions() = 0;
};

} // namespace age::core
