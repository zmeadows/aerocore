#include "Sprite.hpp"

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
    const auto pc = GC->toScreenCoordinates(pos);

    const int x = pc.x;
    const int y = pc.y;
    const int tmpX = GC->toScreenSpan(triangle.baseWidth / 2.0);
    const int tmpY = GC->toScreenSpan(triangle.height / 3.0);

    aatrigonRGBA(GC->renderer, x - tmpX, y + tmpY, x + tmpX, y + tmpY, x, y - 2 * tmpY, rgba.r, rgba.g,
                 rgba.b, rgba.a);
}
