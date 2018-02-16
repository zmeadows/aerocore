#include "Sprite.hpp"

#include <vector>

std::vector<ScreenCoordinates> vtxToScreenCoords(GraphicsContext* GC,
                                              const std::vector<Vector2f>& vertices)
{
    std::vector<ScreenCoordinates> vsc;
    vsc.reserve(vertices.size());

    for (const Vector2f& vtx : vertices) {
        vsc.push_back(GC->toScreenCoordinates({vtx.x, vtx.y}));
    }

    return vsc;
}

Sprite makeIsoTriangleSprite(float baseWidth, float height)
{
    Sprite spr;

    spr.vertices = {
        {-baseWidth / 2.f, -height / 3.f},
        {baseWidth / 2.f, -height / 3.f},
        {0.f, 2.f * height / 3.f}
    };

    return spr;
}

Sprite makeSquareSprite(float width)
{
    Sprite spr;

    spr.vertices = {
        {-width / 2.f, -width / 2.f},
        {width / 2.f, -width / 2.f},
        {width / 2.f, width / 2.f},
        {-width / 2.f, width / 2.f}
    };

    return spr;
}

std::vector<Vector2f> transformVtxs(const std::vector<Vector2f> vertices,
                                    const Position& pos, const Rotation& rot)
{
    std::vector<Vector2f> tmpVertices;
    tmpVertices.reserve(vertices.size());

    const Vector2f posVec = {pos.x, pos.y};
    const float angle = rot.getAngle();

    for (const Vector2f& vtx : vertices) {
        tmpVertices.push_back(vtx.rotated(angle) + posVec);
    }

    return tmpVertices;
}

void draw(GraphicsContext* GC, const Sprite& sprite,
          const Position& pos, const Rotation& rot)
{
    const std::vector<ScreenCoordinates> vsc = vtxToScreenCoords(GC, transformVtxs(sprite.vertices, pos, rot));

    const RGBA rgba = sprite.color;
    const size_t vtxCount = vsc.size();
    // TODO: use new SDL_gpu code
    for (size_t i = 0; i < vtxCount; i++) {
        aalineRGBA(GC->renderer,
                   vsc[i].x, vsc[i].y,
                   vsc[(i + 1) % vtxCount].x, vsc[(i + 1) % vtxCount].y,
                   rgba.r, rgba.g, rgba.b, rgba.a);
    }
}

void scale(Sprite& sprite, float factor) {
    for (Vector2f& vtx : sprite.vertices)
        vtx.scale(factor);
}

void extentAt(Extent& ext, const Sprite& sprite, const Position& pos, const Rotation& rot)
{
    ext.minX = std::numeric_limits<float>::max();
    ext.maxX = std::numeric_limits<float>::lowest();
    ext.minY = std::numeric_limits<float>::max();
    ext.maxY = std::numeric_limits<float>::lowest();

    for (const Vector2f& vtx : transformVtxs(sprite.vertices, pos, rot))
    {
        ext.minX = std::min(ext.minX, vtx.x);
        ext.maxX = std::max(ext.maxX, vtx.x);
        ext.minY = std::min(ext.minY, vtx.y);
        ext.maxY = std::max(ext.maxY, vtx.y);
    }
}

bool isOffScreen(const Sprite& sprite, const Position& pos, const Rotation& rot)
{
    for (const Vector2f& vtx : transformVtxs(sprite.vertices, pos, rot))
    {
        if (std::abs(vtx.x) < 100.f && std::abs(vtx.y) < 100.f) {
            return false;
        }
    }

    return true;
}

