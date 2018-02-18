#include "Base.hpp"

const UUID playerUUID(void) {
    static UUID id;
    return id;
}

float uniform_rand(float min, float max) {
    assert( max > min );
    return static_cast<float>(min + (rand()/static_cast<float>(RAND_MAX)) * (max - min));
};

Extent clip_to_screen(const Extent& ext) {
    Extent new_ext = ext;

    if (ext.minX < -100.f) new_ext.minX = -100.f + 1e-6f;
    if (ext.minY < -100.f) new_ext.minY = -100.f + 1e-6f;
    if (ext.maxX > 100.f) new_ext.maxX = 100.f - 1e-3f;
    if (ext.maxY > 100.f) new_ext.maxY = 100.f - 1e-3f;

    return new_ext;
}
