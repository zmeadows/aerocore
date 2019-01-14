#include "Base.hpp"

#include "UUID.hpp"

#include <algorithm>
#include <assert.h>
#include <vector>

const UUID playerUUID(void) {
    static UUID id;
    return id;
}

void dump(const Extent& extent) {
    std::cout << "MIN X: " << extent.minX << std::endl;
    std::cout << "MAX X: " << extent.maxX << std::endl;
    std::cout << "MIN Y: " << extent.minY << std::endl;
    std::cout << "MAX Y: " << extent.maxY << std::endl;
    std::cout << std::endl;
}

Extent clip_to_screen(const Extent& ext) {
    Extent clipped_ext = ext;

    //@FIXME: avoid these deltas? or consult GraphicsContext and choose delta cleverly?
    if (ext.minX < -100.f) clipped_ext.minX = -100.f + 1e-6f;
    if (ext.minY < -100.f) clipped_ext.minY = -100.f + 1e-6f;
    if (ext.maxX > 100.f) clipped_ext.maxX = 100.f - 1e-3f;
    if (ext.maxY > 100.f) clipped_ext.maxY = 100.f - 1e-3f;

    return clipped_ext;
}

Extent extent_of(const std::vector<v2>& global_vertices)
{
    Extent ext;

    for (size_t i = 0; i < global_vertices.size(); i++)
    {
        ext.minX = std::min(ext.minX, global_vertices[i].x);
        ext.maxX = std::max(ext.maxX, global_vertices[i].x);
        ext.minY = std::min(ext.minY, global_vertices[i].y);
        ext.maxY = std::max(ext.maxY, global_vertices[i].y);
    }

    return ext;
}

Extent extent_of(const LocalVertexBuffer* local_vertices, v2 position, f32 angle) {
    return extent_of(compute_global_vertices(local_vertices, position, angle));
}

bool is_offscreen(const Extent& ext) {
        return ext.maxX < -100.f
            || ext.minX > 100.f
            || ext.maxY < -100.f
            || ext.minY > 100.f;
}

std::vector<v2> compute_global_vertices( const LocalVertexBuffer* lvb
                                       , const v2 position_offset
                                       , const float rotation_angle)
{
    std::vector<v2> gvb;
    gvb.reserve(lvb->count);

    for (size_t i = 0; i < lvb->count; i++) {
        gvb.push_back((*lvb)[i].rotated(rotation_angle) + position_offset);
    }

    return gvb;
}

