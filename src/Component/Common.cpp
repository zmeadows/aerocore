#include "Component/Common.hpp"

#include "Extent.hpp"

v2 orientation_of(const Entity& entity) {
    return { std::cos(entity.angle), std::sin(entity.angle) };
}

bool is_offscreen(const Entity& entity) {
    return is_offscreen(entity.extent);
}

