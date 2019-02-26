#pragma once

#include "Engine/System.hpp"
#include "unstd/DynamicArray.hpp"
#include "Component/Common.hpp"

struct InvincibilitySystem final : System {
    InvincibilitySystem() : System("Invincibility") {}
    void run(ComponentManager* CM, f32 dt);
    DynamicArray<UUID> finished;
    SUBSCRIBE(Sprite, Invincibility);
};

