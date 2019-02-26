#pragma once

#include "Engine/System.hpp"

struct Entity;
struct Sprite;
class GraphicsContext;

struct DrawSystem final : System {
    GraphicsContext* GC;
    DrawSystem(GraphicsContext* _GC) : GC(_GC), System("Draw") {}
    void run(ComponentManager* CM, float dt);

    SUBSCRIBE(Entity, Sprite);
};

