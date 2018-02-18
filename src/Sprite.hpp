#pragma once

#include <SDL2/SDL.h>

#include "Base.hpp"
#include "Sprite.hpp"
#include "BoundingSurface.hpp"
#include "GraphicsContext.hpp"

//TODO: struct SpritePool

std::vector<v2> make_iso_triangle_vertices(float baseWidth, float height);
std::vector<v2> make_square_vertices(float width);
std::vector<v2> transform_vertices(const std::vector<v2>& vertices, const v2& pos, float angle);

void draw(GraphicsContext* GC,
          const std::vector<v2>& vertices,
          const v2& pos,
          const float angle,
          const SDL_Color& rgba);

Extent extent_at(const CoreData& cd);
Extent extent_at(const std::vector<v2>& vertices, const v2& pos, const float angle);

bool is_offscreen(const CoreData& cd);
bool is_offscreen(const std::vector<v2>& vertices, const v2& pos, const float rot);

