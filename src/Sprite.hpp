#pragma once

#include <SDL2/SDL.h>

#include "Entity.hpp"
#include "GraphicsContext.hpp"

void assign_iso_triangle_vertices(Entity& entity, float base_width, float height);

void draw(GraphicsContext& GC, const Entity& entity);

void populate_global_vertices(const LocalVertexBuffer& lvb, GlobalVertexBuffer& gvb,
                              const v2 position_offset, const float rotation_angle);

