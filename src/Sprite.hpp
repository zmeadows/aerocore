#pragma once

#include <SDL2/SDL.h>

#include "Base.hpp"
#include "GraphicsContext.hpp"

//TODO: struct SpritePool

std::vector<v2> make_iso_triangle_vertices(float baseWidth, float height);
std::vector<v2> make_square_vertices(float width);
std::vector<v2> transform_vertices(const std::vector<v2>& vertices, const v2& pos, float angle);

std::vector<std::vector<v2>> transform_nested_vertices(
    const std::vector<std::vector<v2>>& nested_vertices,
    const v2& pos, const float angle);

void draw(GraphicsContext* GC,
          const std::vector<v2>& global_vertices,
          const SDL_Color& rgba);

Extent extent_of(const Entity& cd);
Extent extent_at(const std::vector<v2>& global_vertices);

//@CLARITY: give these distinct names
bool is_offscreen(const Extent& ext);
bool is_offscreen(const Entity& cd);
bool is_offscreen(const std::vector<v2>& global_vertices);

