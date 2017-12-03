#include "Sprite.hpp"

#include <vector>

void SquareSprite::draw(GraphicsContext* GC, const Position& pos, const Rotation& rot) const
{
    const auto pc = GC->toScreenCoordinates(pos);

    const Sint16 x = pc.x;
    const Sint16 y = pc.y;
    const Sint16 hw = GC->toScreenSpan(square.width / 2.0);

    boxRGBA(GC->renderer, x - hw, y - hw, x + hw, y + hw, rgba.r, rgba.g, rgba.b, 255);
}

void IsoTriangleSprite::draw(GraphicsContext* GC, const Position& pos, const Rotation& rot) const
{
    std::vector<Vector2f> vertices = triangle.getTransRotVertices(pos, rot);
    assert(vertices.size() == 3);

    std::vector<ScreenCoordinates> vsc;
    vsc.reserve(3);

    for (const auto& vtx : vertices) {
        vsc.push_back(GC->toScreenCoordinates({vtx.x(), vtx.y()}));
    }

    aalineRGBA(GC->renderer, vsc[0].x, vsc[0].y, vsc[1].x, vsc[1].y, rgba.r, rgba.g, rgba.b, rgba.a);
    aalineRGBA(GC->renderer, vsc[1].x, vsc[1].y, vsc[2].x, vsc[2].y, rgba.r, rgba.g, rgba.b, rgba.a);
    aalineRGBA(GC->renderer, vsc[2].x, vsc[2].y, vsc[0].x, vsc[0].y, rgba.r, rgba.g, rgba.b, rgba.a);
}
