#pragma once

#include "System.hpp"
#include "unstd/DynamicArray.hpp"
#include "Component/Common.hpp"

struct InvincibilitySystem {
    System base = System("Invincibility");
    DynamicArray<UUID> finished;
    SUBSCRIBE(Sprite, Invincibility);
};

void run(InvincibilitySystem&, f32 dt);
