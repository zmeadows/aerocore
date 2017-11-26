#include "Sprite.hpp"

void SquareSprite::draw(GraphicsContext* GC, const Position& pos) const {
    const auto pc = GC->toPixelCoords(pos);

    const int x = pc.first;
    const int y = pc.second;
    const int hw = GC->toPixelSpan(width / 2.0);

    boxRGBA(GC->renderer, x - hw, y - hw, x + hw, y + hw, rgba.r, rgba.g, rgba.b, 255);
}