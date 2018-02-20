#include "Sprite.hpp"

#include <vector>

std::vector<v2> make_iso_triangle_vertices(float baseWidth, float height)
{
    return {
        {-baseWidth / 2.f, -height / 3.f},
        {0.f, 2.f * height / 3.f},
        {baseWidth / 2.f, -height / 3.f}
    };
}

std::vector<v2> make_square_vertices(float width)
{
    return {
        {-width / 2.f, -width / 2.f},
        {-width / 2.f, width / 2.f},
        {width / 2.f, width / 2.f},
        {width / 2.f, -width / 2.f}
    };
}

std::vector<v2> transform_vertices(const std::vector<v2>& vertices,
                                    const v2& pos, const float angle)
{
    std::vector<v2> tmpVertices;
    tmpVertices.reserve(vertices.size());

    const v2 posVec = {pos.x, pos.y};

    for (const v2& vtx : vertices) {
        tmpVertices.push_back(vtx.rotated(angle) + posVec);
    }

    return tmpVertices;
}

std::vector<v2> transform_vertices(const Entity& CD) {
    return transform_vertices(CD.vertices, CD.pos, CD.angle);
}

void draw(GraphicsContext* GC,
          const std::vector<v2>& vertices,
          const v2& pos,
          const float angle,
          const SDL_Color& rgba)
{
    const std::vector<v2> trans_vertices = transform_vertices(vertices, pos, angle);
    const size_t count = trans_vertices.size();

    // GPU_CircleFilled(GC->renderer,
    //                  GC->to_screen_coords(trans_vertices[0]).x,
    //                  GC->to_screen_coords(trans_vertices[0]).y,
    //                  20,
    //                  { 0,0,255,255});

    for (size_t i = 0; i < count; i++) {
        ScreenCoordinates sc1 = GC->to_screen_coords(trans_vertices[i]);
        ScreenCoordinates sc2 = GC->to_screen_coords(trans_vertices[(i+1) % count]);

        GPU_Line(GC->renderer,
                 sc1.x, sc1.y,
                 sc2.x, sc2.y,
                 rgba);
    }
}

Extent extent_of(const Entity& cd) {
    return extent_at(cd.vertices, cd.pos, cd.angle);
}

Extent extent_at(const std::vector<v2>& vertices, const v2& pos, const float angle)
{
    Extent ext;

    ext.minX = std::numeric_limits<float>::max();
    ext.maxX = std::numeric_limits<float>::lowest();
    ext.minY = std::numeric_limits<float>::max();
    ext.maxY = std::numeric_limits<float>::lowest();

    for (const v2& vtx : transform_vertices(vertices, pos, angle))
    {
        ext.minX = std::min(ext.minX, vtx.x);
        ext.maxX = std::max(ext.maxX, vtx.x);
        ext.minY = std::min(ext.minY, vtx.y);
        ext.maxY = std::max(ext.maxY, vtx.y);
    }

    return ext;
}

bool is_offscreen(const Entity& cd) {
    return is_offscreen(cd.vertices, cd. pos, cd.angle);
}

bool is_offscreen(const Extent& ext) {
        return ext.maxX < -100.f
            && ext.minX > 100.f
            && ext.maxY < -100.f
            && ext.minY > 100.f;
}


bool is_offscreen(const std::vector<v2>& vertices, const v2& pos, const float angle)
{
    for (const v2& vtx : transform_vertices(vertices, pos, angle))
    {
        if (std::abs(vtx.x) < 100.f && std::abs(vtx.y) < 100.f) {
            return false;
        }
    }

    return true;
}

