#pragma once

#include <algorithm>
#include <array>
#include <vector>

struct LocalVertexBuffer;
struct GlobalVertexBuffer;
struct v2;

bool is_convex(const LocalVertexBuffer& vertices);

v2 recenter(LocalVertexBuffer& local_vertices);

v2 average_vector(const v2* vertices, const size_t vertex_count);
v2 average_vector(const LocalVertexBuffer& global_vertices);
v2 average_vector(const GlobalVertexBuffer& global_vertices);

