#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_gpu.h>

class GraphicsContext;
struct Entity;
struct LocalVertexBuffer;
struct GlobalVertexBuffer;
struct v2;


void assign_iso_triangle_vertices(Entity& entity, float base_width, float height);

void draw(GraphicsContext& GC, const Entity& entity);

void populate_global_vertices(const LocalVertexBuffer& lvb, GlobalVertexBuffer& gvb,
                              const v2 position_offset, const float rotation_angle);

struct SVGParsePathResult {
    LocalVertexBuffer vertices;
    v2 sprite_offset;
};

SVGParsePathResult parse_svg_path(const char* filepath);
