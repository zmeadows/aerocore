#pragma once

#include "Base.hpp"
#include "Vector2D.hpp"

#include <algorithm>
#include <array>
#include <vector>

int modulo(int num, int mod);

v2 average_vec(const std::vector<v2>& vertices);
std::vector<std::vector<size_t>> decompose_into_triangle_indices(const std::vector<v2>& vertices);
bool pnpoly(const std::vector<v2>& vertices, const v2& test);
bool is_convex(const std::vector<v2>& vertices);

