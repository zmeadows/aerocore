#pragma once

#include <SDL.h>
#include <SDL_gpu.h>

class GraphicsContext;
struct Entity;
struct LocalVertexBuffer;
struct v2;


//void assign_iso_triangle_vertices(Entity& entity, float base_width, float height);

void draw(GraphicsContext& GC, const Sprite& sprite, v2 pos, float angle);

struct SVGParsePathResult {
    LocalVertexBuffer vertices;
    v2 sprite_offset;
};

SVGParsePathResult parse_svg_path(const char* filepath);
