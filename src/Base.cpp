#include "Base.hpp"

const UUID playerUUID(void) {
    static UUID id;
    return id;
}

float uniform_rand(f32 min, f32 max) {
    assert( max > min );
    return min + static_cast<float>(rand())/static_cast<float>(RAND_MAX) * (max - min);
};


Extent clip_to_screen(const Extent& ext) {
    Extent clipped_ext = ext;

    //@FIXME: avoid these deltas? or consult GraphicsContext and choose delta cleverly?
    if (ext.minX < -100.f) clipped_ext.minX = -100.f + 1e-6f;
    if (ext.minY < -100.f) clipped_ext.minY = -100.f + 1e-6f;
    if (ext.maxX > 100.f) clipped_ext.maxX = 100.f - 1e-3f;
    if (ext.maxY > 100.f) clipped_ext.maxY = 100.f - 1e-3f;

    return clipped_ext;
}
