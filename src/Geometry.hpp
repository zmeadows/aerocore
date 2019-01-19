#pragma once

#include "Typedef.hpp"

#include <algorithm>
#include <array>
#include <vector>

struct LocalVertexBuffer;
struct v2;

bool is_convex(const std::vector<v2>& vertices);
bool is_convex(const LocalVertexBuffer& vertices);
bool is_convex(const v2* vertices, u32 vertex_count);
v2 recenter(LocalVertexBuffer& local_vertices);

v2 average_vector(const v2* vertices, const size_t vertex_count);
v2 average_vector(const LocalVertexBuffer& global_vertices);

