#pragma once

#include "Vector2D.hpp"
#include "unstd/types.hpp"

#include <vector>

namespace {
// no need to keep this number very small, they won't be stored contiguously anywhere
const u32 LOCAL_VERTEX_BUFFER_SIZE = 256;
}

struct LocalVertexBuffer {
    v2 data[LOCAL_VERTEX_BUFFER_SIZE];
    u32 count = 0;

    v2 operator[](u32 idx) const { return data[idx]; }
    v2& operator[](u32 idx) { return data[idx]; }

    v2* begin() { return &data[0]; }
    v2* end() { return &data[count]; }
    const v2* begin() const { return &data[0]; }
    const v2* end() const { return &data[count]; }
};

std::vector<v2>
compute_global_vertices(const LocalVertexBuffer* lvb, const v2 position_offset, const float rotation_angle);
