#include "Extent.hpp"

#include <algorithm> //TODO: remove dependence on this
#include <cstdio>

#include "Vector2D.hpp"
#include "LocalVertexBuffer.hpp"

void dump(const Extent& extent) {
    printf("X: (%f, %f) Y: (%f, %f)\n",
           extent.minX, extent.maxX,
           extent.minY, extent.maxY);
}

Extent clip_to_screen(const Extent& ext) {
    Extent clipped_ext = ext;

    //@FIXME: avoid these deltas? or consult GraphicsContext and choose delta cleverly?
    if (ext.minX < -100.f)
        clipped_ext.minX = -100.f + 1e-6f;
    if (ext.minY < -100.f)
        clipped_ext.minY = -100.f + 1e-6f;
    if (ext.maxX > 100.f)
        clipped_ext.maxX = 100.f - 1e-3f;
    if (ext.maxY > 100.f)
        clipped_ext.maxY = 100.f - 1e-3f;

    return clipped_ext;
}

Extent extent_of(const std::vector<v2>& global_vertices) {
    Extent ext;

    for (size_t i = 0; i < global_vertices.size(); i++) {
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
    return ext.maxX < -100.f || ext.minX > 100.f || ext.maxY < -100.f || ext.minY > 100.f;
}
