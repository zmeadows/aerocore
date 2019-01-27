#pragma once

#include "System.hpp"

#include "Component/Common.hpp"

struct DrawSystem {
    System base = System("Draw");
    SUBSCRIBE(Entity, Sprite);
};

void run(DrawSystem&);
