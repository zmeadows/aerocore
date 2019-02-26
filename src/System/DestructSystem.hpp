#pragma once

#include "Engine/System.hpp"
#include "Component/Common.hpp"

struct DestructSystem final : System {
    DestructSystem() : System("Destruct") {}
    DynamicArray<UUID> followed_copy;
    void run(ComponentManager* CM, float dt);
    SUBSCRIBE(DestructTag);
};
