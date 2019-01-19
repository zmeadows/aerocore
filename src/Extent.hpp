#pragma once

#include "Typedef.hpp"

#include <limits>
#include <vector>

struct LocalVertexBuffer;
struct v2;

struct Extent {
    f32 minX = std::numeric_limits<f32>::max();
    f32 maxX = std::numeric_limits<f32>::lowest();
    f32 minY = std::numeric_limits<f32>::max();
    f32 maxY = std::numeric_limits<f32>::lowest();
};

Extent clip_to_screen(const Extent& ext);

void dump(const Extent& extent);

bool is_offscreen(const Extent& ext);
Extent extent_of(const std::vector<v2>& global_vertices);
Extent extent_of(const LocalVertexBuffer* local_vertices, v2 position, f32 angle);
