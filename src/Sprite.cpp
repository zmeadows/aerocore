#include "Sprite.hpp"
#include "Geometry.hpp"
#include "Entity.hpp"

#include <vector>

void assign_iso_triangle_vertices(Entity& entity, float base_width, float height)
{
    entity.local_vertices.count = 3;
    entity.local_vertices[0] = {0.f, base_width / 2.f};
    entity.local_vertices[1] = {height, 0.f};
    entity.local_vertices[2] = {0.f, -base_width / 2.f};

    recenter(entity.local_vertices);
}

void populate_global_vertices(const LocalVertexBuffer& lvb, GlobalVertexBuffer& gvb,
                              const v2 position_offset, const float rotation_angle)
{
    for (size_t i = 0; i < lvb.count; i++)
        gvb[i] = lvb[i].rotated(rotation_angle) + position_offset;
    gvb.count = lvb.count;
}

void draw(GraphicsContext& GC, const Entity& entity)
{

    // v2 orientation = orientation_of(entity);
    // orientation.scale(10.f);
    // v2 orientation_dot = entity.pos + orientation;

    // GPU_CircleFilled(GC->renderer,
    //                  GC->to_screen_coords(orientation_dot).x,
    //                  GC->to_screen_coords(orientation_dot).y,
    //                  4,
    //                  { 30,225,30,175});

    // GPU_Line(GC->renderer,
    //                  GC->to_screen_coords(entity.pos).x,
    //                  GC->to_screen_coords(entity.pos).y,
    //                  GC->to_screen_coords(orientation_dot).x,
    //                  GC->to_screen_coords(orientation_dot).y,
    //                  { 30,225,30,175});

    // GPU_CircleFilled(GC->renderer,
    //                  GC->to_screen_coords(global_vertices[0]).x,
    //                  GC->to_screen_coords(global_vertices[0]).y,
    //                  10,
    //                  { 0,0,255,255});

    const size_t vtx_count = entity.global_vertices.count;

    for (size_t i = 0; i < vtx_count; i++) {
        ScreenCoordinates sc1 = GC.to_screen_coords(entity.global_vertices[i]);
        ScreenCoordinates sc2 = GC.to_screen_coords(entity.global_vertices[(i+1) % vtx_count]);

        GPU_Line(GC.renderer,
                 sc1.x, sc1.y,
                 sc2.x, sc2.y,
                 entity.color);
    }
}


