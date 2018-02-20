#pragma once

#include "Base.hpp"
#include "Vector2D.hpp"

#include <algorithm>
#include <array>
#include <vector>

v2 average_vec(const std::vector<v2>& vertices);
std::vector<std::vector<v2>> decompose(const std::vector<v2>& vertices);
bool pnpoly(const std::vector<v2>& vertices, const v2& test);
bool is_convex(const std::vector<v2>& vertices);

