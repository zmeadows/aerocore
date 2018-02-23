#pragma once

#include "Base.hpp"
#include "Vector2D.hpp"
#include "BoundingSurface.hpp"

#include <algorithm>
#include <array>
#include <vector>

int modulo(int num, int mod);

bool is_convex(const v2* vertices, const size_t vertex_count);

void recenter(LocalVertexBuffer& local_vertices);

v2 average_vector(const v2* vertices, const size_t vertex_count);

inline v2 average_vector(const LocalVertexBuffer& global_vertices) {
    return average_vector(global_vertices.data, global_vertices.count);
}

inline v2 average_vector(const GlobalVertexBuffer& global_vertices) {
    return average_vector(global_vertices.data, global_vertices.count);
}

bool pnpoly(const v2* vertices, const size_t nvert, const v2& test);

PolygonDecomposition
decompose_local_vertices(const LocalVertexBuffer& local_vertices);
