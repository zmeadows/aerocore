#include "Base.hpp"

#include "UUID.hpp"

const UUID playerUUID(void) {
    static UUID id;
    return id;
}

float uniform_rand(float min, float max) {
    assert( max > min );
    return min + static_cast<float>(rand())/static_cast<float>(RAND_MAX) * (max - min);
};


Extent clip_to_screen(const Extent& ext) {
    Extent clipped_ext = ext;

    //@FIXME: avoid these deltas? or consult GraphicsContext and choose delta cleverly?
    //@TODO: what happens when the entity is off screen completely?
    if (ext.minX < -100.f) clipped_ext.minX = -100.f + 1e-6f;
    if (ext.minY < -100.f) clipped_ext.minY = -100.f + 1e-6f;
    if (ext.maxX > 100.f) clipped_ext.maxX = 100.f - 1e-3f;
    if (ext.maxY > 100.f) clipped_ext.maxY = 100.f - 1e-3f;

    return clipped_ext;
}

Extent extent_of(const GlobalVertexBuffer& global_vertices)
{
    Extent ext;

    for (size_t i = 0; i < global_vertices.count; i++)
    {
        ext.minX = std::min(ext.minX, global_vertices[i].x);
        ext.maxX = std::max(ext.maxX, global_vertices[i].x);
        ext.minY = std::min(ext.minY, global_vertices[i].y);
        ext.maxY = std::max(ext.maxY, global_vertices[i].y);
    }

    return ext;
}


bool is_offscreen(const Extent& ext) {
        return ext.maxX < -100.f
            || ext.minX > 100.f
            || ext.maxY < -100.f
            || ext.minY > 100.f;
}
