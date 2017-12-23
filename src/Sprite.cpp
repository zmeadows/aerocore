#include "Sprite.hpp"

#include <vector>

void SquareSprite::draw(GraphicsContext* GC, const Position& pos, const Rotation& rot) const {
    std::vector<Vector2f> vertices = square.getTransRotVertices(pos, rot);
    assert(vertices.size() == 4);

    std::vector<ScreenCoordinates> vsc;
    vsc.reserve(4);

    for (const auto& vtx : vertices) {
        vsc.push_back(GC->toScreenCoordinates({vtx.x, vtx.y}));
    }

    // lineRGBA(GC->renderer, vsc[0].x, vsc[0].y, vsc[1].x, vsc[1].y, rgba.r, rgba.g, rgba.b, rgba.a);
    // lineRGBA(GC->renderer, vsc[1].x, vsc[1].y, vsc[2].x, vsc[2].y, rgba.r, rgba.g, rgba.b, rgba.a);
    // lineRGBA(GC->renderer, vsc[2].x, vsc[2].y, vsc[3].x, vsc[3].y, rgba.r, rgba.g, rgba.b, rgba.a);
    // lineRGBA(GC->renderer, vsc[3].x, vsc[3].y, vsc[0].x, vsc[0].y, rgba.r, rgba.g, rgba.b, rgba.a);
}

void IsoTriangleSprite::draw(GraphicsContext* GC, const Position& pos, const Rotation& rot) const {
    std::vector<Vector2f> vertices = triangle.getTransRotVertices(pos, rot);
    assert(vertices.size() == 3);

    std::vector<ScreenCoordinates> vsc;
    vsc.reserve(3);

    for (const auto& vtx : vertices) {
        vsc.push_back(GC->toScreenCoordinates({vtx.x, vtx.y}));
    }

    // lineRGBA(GC->renderer, vsc[0].x, vsc[0].y, vsc[1].x, vsc[1].y, rgba.r, rgba.g, rgba.b, rgba.a);
    // lineRGBA(GC->renderer, vsc[1].x, vsc[1].y, vsc[2].x, vsc[2].y, rgba.r, rgba.g, rgba.b, rgba.a);
    // lineRGBA(GC->renderer, vsc[2].x, vsc[2].y, vsc[0].x, vsc[0].y, rgba.r, rgba.g, rgba.b, rgba.a);
}
